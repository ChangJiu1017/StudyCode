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
#define EXPECTED_CRTC_ID 44
#define WIDTH 1920
#define HEIGHT 1080
#define BPP 32

int main() {
    int fd = open("/dev/dri/card0", O_RDWR | O_CLOEXEC);
    if (fd < 0) {
        perror("open /dev/dri/card0");
        return 1;
    }

    if (drmSetMaster(fd) < 0) {
        perror("drmSetMaster");
        close(fd);
        return 1;
    }

    drmModeConnector *conn = drmModeGetConnector(fd, CONNECTOR_ID);
    if (!conn || conn->connection != DRM_MODE_CONNECTED || conn->count_modes == 0) {
        fprintf(stderr, "Connector 55 not ready or disconnected.\n");
        return 1;
    }

    drmModeModeInfo mode = conn->modes[0];

    int crtc_id = EXPECTED_CRTC_ID;

    // 创建 dumb buffer
    struct drm_mode_create_dumb create = {0};
    create.width = mode.hdisplay;
    create.height = mode.vdisplay;
    create.bpp = BPP;

    if (ioctl(fd, DRM_IOCTL_MODE_CREATE_DUMB, &create) < 0) {
        perror("CREATE_DUMB");
        return 1;
    }

    uint32_t fb_id;
    if (drmModeAddFB(fd, create.width, create.height, 24, 32, create.pitch, create.handle, &fb_id)) {
        perror("drmModeAddFB");
        return 1;
    }

    struct drm_mode_map_dumb map = { .handle = create.handle };
    if (ioctl(fd, DRM_IOCTL_MODE_MAP_DUMB, &map)) {
        perror("MAP_DUMB");
        return 1;
    }

    void *fb = mmap(0, create.size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, map.offset);
    if (fb == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    // 填充蓝色
    uint32_t *pix = fb;
    for (int i = 0; i < (create.width * create.height); i++) {
        pix[i] = 0xFF0000FF; 
    }

    if (drmModeSetCrtc(fd, crtc_id, fb_id, 0, 0, &conn->connector_id, 1, &mode)) {
        perror("drmModeSetCrtc");
        return 1;
    }


    drmDropMaster(fd); // 主动释放控制权

    sleep(10); 

    // 清理
    munmap(fb, create.size);
    drmModeFreeConnector(conn);
    close(fd);

    return 0;
}
