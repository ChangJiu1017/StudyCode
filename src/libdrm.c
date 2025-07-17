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

#define CONNECTOR_ID 55
#define CRTC_ID 44
#define WIDTH 1920
#define HEIGHT 1080
#define BPP 32

int main() {
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

    uint32_t *pixel = fb;
    int pitch_words = create.pitch / 4;
    uint32_t red = 0xFFFF0000;
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            pixel[y * pitch_words + x] = red;

    if (drmModeSetCrtc(fd, CRTC_ID, fb_id, 0, 0, &conn->connector_id, 1, &mode)) {
        perror("drmModeSetCrtc (first)");
        return 1;
    }
    printf("[INFO] First frame shown (red).\n");

    // 放弃 master 权限
    if (drmDropMaster(fd) != 0) {
        perror("drmDropMaster");
    } else {
        printf("[INFO] Dropped DRM master.\n");
    }

    sleep(10);

    // 显示其他颜色
    uint32_t colors[] = {
        0xFF00FF00, // green
        0xFF0000FF, // blue
        0xFFFFFFFF  // white
    };

    for (int i = 0; i < sizeof(colors)/sizeof(colors[0]); i++) {
        printf("\n[INFO] Attempting to display color 0x%08X...\n", colors[i]);

        for (int y = 0; y < HEIGHT; y++)
            for (int x = 0; x < WIDTH; x++)
                pixel[y * pitch_words + x] = colors[i];

        int is_master = drmIsMaster(fd);
        printf("[INFO] drmIsMaster: %s\n", is_master ? "YES" : "NO");

        if (drmModeSetCrtc(fd, CRTC_ID, fb_id, 0, 0, &conn->connector_id, 1, &mode) != 0) {
            fprintf(stderr, "[ERROR] drmModeSetCrtc failed: errno=%d (%s)\n", errno, strerror(errno));
        } else {
            printf("[SUCCESS] Color shown successfully.\n");
        }

        sleep(10);
    }

    // 清理
    munmap(fb, create.size);
    drmModeRmFB(fd, fb_id);
    destroy.handle = handle;
    drmIoctl(fd, DRM_IOCTL_MODE_DESTROY_DUMB, &destroy);
    drmModeFreeConnector(conn);
    drmModeFreeResources(res);
    close(fd);

    return 0;
}