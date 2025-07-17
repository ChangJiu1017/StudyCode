#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <xf86drm.h>
#include <xf86drmMode.h>
#include <time.h>

#define CONNECTOR_ID 55
#define CRTC_ID 44
#define WIDTH 1280
#define HEIGHT 768
#define BPP 32
#define PIXEL_SIZE 4
#define FRAME_SIZE (WIDTH * HEIGHT * PIXEL_SIZE)

#define SHMEM_PHYS_ADDR 0xd0100000UL
#define SHMEM_SIZE FRAME_SIZE

static void msleep(int ms) {
    struct timespec ts = {
        .tv_sec = ms / 1000,
        .tv_nsec = (ms % 1000) * 1000000
    };
    nanosleep(&ts, NULL);
}

int main() {
    // 打开 DRM 设备
    int fd = open("/dev/dri/card0", O_RDWR | O_CLOEXEC);
    if (fd < 0) {
        perror("open /dev/dri/card0");
        return 1;
    }

    drmModeRes *res = drmModeGetResources(fd);
    drmModeConnector *conn = drmModeGetConnector(fd, CONNECTOR_ID);
    if (!conn || conn->connection != DRM_MODE_CONNECTED || conn->count_modes == 0) {
        fprintf(stderr, "Connector %d is not connected or has no modes\n", CONNECTOR_ID);
        return 1;
    }

    drmModeModeInfo mode = conn->modes[0];

    uint32_t fb_id, handle;
    struct drm_mode_create_dumb create = {0};
    struct drm_mode_map_dumb map = {0};
    struct drm_mode_destroy_dumb destroy = {0};

    create.width = WIDTH;
    create.height = HEIGHT;
    create.bpp = BPP;
    if (drmIoctl(fd, DRM_IOCTL_MODE_CREATE_DUMB, &create) < 0) {
        perror("CREATE_DUMB");
        return 1;
    }
    handle = create.handle;

    if (drmModeAddFB(fd, WIDTH, HEIGHT, 24, 32, create.pitch, handle, &fb_id)) {
        perror("drmModeAddFB");
        return 1;
    }

    map.handle = handle;
    if (drmIoctl(fd, DRM_IOCTL_MODE_MAP_DUMB, &map)) {
        perror("MAP_DUMB");
        return 1;
    }

    void *fb = mmap(0, create.size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, map.offset);
    if (fb == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    // 显示第一次画面
    if (drmModeSetCrtc(fd, CRTC_ID, fb_id, 0, 0, &conn->connector_id, 1, &mode)) {
        perror("drmModeSetCrtc");
        return 1;
    }
    printf("[INFO] Framebuffer setup complete.\n");

    int mem_fd = open("/dev/mem", O_RDONLY | O_SYNC);
    if (mem_fd < 0) {
        perror("open /dev/mem");
        return 1;
    }

    void *shmem = mmap(NULL, SHMEM_SIZE, PROT_READ, MAP_SHARED, mem_fd, SHMEM_PHYS_ADDR);
    if (shmem == MAP_FAILED) {
        perror("mmap /dev/mem");
        return 1;
    }

    printf("[INFO] Entering display loop...\n");

    // 主循环不断拷贝共享内存数据到 framebuffer
    while (1) {
        uint8_t *dst = (uint8_t *)fb;
        uint8_t *src = (uint8_t *)shmem;

        memcpy(fb, shmem, FRAME_SIZE);
    }

    munmap(fb, create.size);
    munmap(shmem, SHMEM_SIZE);
    close(mem_fd);
    drmModeRmFB(fd, fb_id);
    destroy.handle = handle;
    drmIoctl(fd, DRM_IOCTL_MODE_DESTROY_DUMB, &destroy);
    drmModeFreeConnector(conn);
    drmModeFreeResources(res);
    close(fd);

    return 0;
}