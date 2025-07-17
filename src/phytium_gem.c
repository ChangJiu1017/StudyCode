/* carve‑out 常量 */
#define SHARED_FB_START  0xD0100000UL
#define SHARED_FB_SIZE   0x01000000UL

#define MEMORY_TYPE_SHARED_FB  BIT(3)

static bool force_shared_fb = true;
static int count = 0;

/* ------------------------------------------------------------ */
struct phytium_gem_object *phytium_gem_create_object2(struct drm_device *dev,
                                                    unsigned long size)
{
    count++;
    DRM_ERROR("phytium_gem_create_object: count : %d\n,", count);
    struct phytium_gem_object *obj = NULL;
    struct phytium_display_private *priv = dev->dev_private;
    struct page *page = NULL;
    int ret = 0;

    obj = kzalloc(sizeof(*obj), GFP_KERNEL);
    if (!obj)
        return ERR_PTR(-ENOMEM);

    ret = drm_gem_object_init(dev, &obj->base, size);
    if (ret)
        goto err_free_obj;

    /* ========== ① 优先：如果本次强制共享显存 ========== */
   // if (force_shared_fb) {

   if (count == 7) {
        force_shared_fb = false; /* 复位，防止影响后续调用 */
	DRM_ERROR("phytium_gem_create_object: force_shared_fb : %d\n,", force_shared_fb);
        if (size > SHARED_FB_SIZE) {
            DRM_ERROR("shared_fb too small: request 0x%lx > 0x%x\n",
                      size, SHARED_FB_SIZE);
            ret = -EINVAL;
            goto err_release;
        }

        obj->vaddr = ioremap_wc(SHARED_FB_START, size);
        if (!obj->vaddr) {
            ret = -ENOMEM;
            goto err_release;
        }

        obj->iova = dma_map_resource(dev->dev, SHARED_FB_START, size,
                                     DMA_BIDIRECTIONAL,
                                     DMA_ATTR_SKIP_CPU_SYNC);
        if (dma_mapping_error(dev->dev, obj->iova)) {
            iounmap(obj->vaddr);
            ret = -EIO;
            goto err_release;
        }

        obj->phys_addr   = SHARED_FB_START;
        obj->memory_type = MEMORY_TYPE_SHARED_FB;
        obj->size        = size;
        goto done;
    }


    /* ========== ② 原有 VRAM / carve‑out / unified 逻辑 ========== */
    if (priv->support_memory_type & (MEMORY_TYPE_VRAM_WC | MEMORY_TYPE_VRAM_DEVICE)) {
        ret = phytium_memory_pool_alloc(priv, &obj->vaddr,
                                        &obj->phys_addr, size);
        if (ret)
            goto err_release;
        obj->iova = obj->phys_addr;
        obj->memory_type = priv->support_memory_type;
        priv->mem_state[PHYTIUM_MEM_VRAM_ALLOC] += size;
    }
    else if (priv->support_memory_type & MEMORY_TYPE_SYSTEM_CARVEOUT) {
        ret = phytium_memory_pool_alloc(priv, &obj->vaddr,
                                        &obj->phys_addr, size);
        if (ret)
            goto err_release;
        page = pfn_to_page(__phys_to_pfn(obj->phys_addr));
        obj->iova = dma_map_page(dev->dev, page, 0, size, DMA_TO_DEVICE);
        if (dma_mapping_error(dev->dev, obj->iova)) {
            phytium_memory_pool_free(priv, obj->vaddr, size);
            ret = -ENOMEM;
            goto err_release;
        }
        obj->memory_type = MEMORY_TYPE_SYSTEM_CARVEOUT;
        priv->mem_state[PHYTIUM_MEM_SYSTEM_CARVEOUT_ALLOC] += size;
    }
    else if (priv->support_memory_type & MEMORY_TYPE_SYSTEM_UNIFIED) {
        obj->vaddr = dma_alloc_attrs(dev->dev, size, &obj->iova,
                                     GFP_KERNEL, 0);
        if (!obj->vaddr) {
            ret = -ENOMEM;
            goto err_release;
        }
        obj->memory_type = MEMORY_TYPE_SYSTEM_UNIFIED;
        priv->mem_state[PHYTIUM_MEM_SYSTEM_UNIFIED_ALLOC] += size;
    }
    else {
        ret = -ENOMEM;
        goto err_release;
    }

done:
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0))
    obj->base.funcs = &phytium_drm_gem_object_funcs;
#endif

    list_add_tail(&obj->list, &priv->gem_list_head);
    DRM_DEBUG_KMS("gem: type=%d iova=0x%pa size=0x%lx\n",
                  obj->memory_type, &obj->iova, obj->size);
    return obj;

err_release:
    drm_gem_object_release(&obj->base);
err_free_obj:
    kfree(obj);
    return ERR_PTR(ret);
}