#include <stdio.h>
#include <syslog.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <bcm_host.h>

int FBCP_process(void(*cb_func)(unsigned char* buff),int screen_width,int screen_height) {
    DISPMANX_DISPLAY_HANDLE_T display;
    DISPMANX_MODEINFO_T display_info;
    DISPMANX_RESOURCE_HANDLE_T screen_resource;
    VC_IMAGE_TRANSFORM_T transform;
    uint32_t image_prt;
    VC_RECT_T rect1;
    int ret;
    unsigned char *fbp;

    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;

    fbp = (unsigned char*)malloc(screen_width * screen_height * 2);

    bcm_host_init();

    display = vc_dispmanx_display_open(0);
    if (!display) {
        return -1;
    }
    ret = vc_dispmanx_display_get_info(display, &display_info);
    if (ret) {
        return -1;
    }

    screen_resource = vc_dispmanx_resource_create(VC_IMAGE_RGB565, screen_width, screen_height, &image_prt);
    if (!screen_resource) {
        vc_dispmanx_display_close(display);
        return -1;
    }

    vc_dispmanx_rect_set(&rect1, 0, 0, screen_width, screen_height);

    while (1) {
      vc_dispmanx_snapshot(display, screen_resource, (DISPMANX_TRANSFORM_T)0);
        vc_dispmanx_resource_read_data(screen_resource, &rect1, fbp, screen_width * 16 / 8);
	cb_func(fbp);

    }

    munmap(fbp, finfo.smem_len);
    vc_dispmanx_resource_delete(screen_resource);
    vc_dispmanx_display_close(display);
}

