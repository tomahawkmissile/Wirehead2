# Wirehead 2
### Experimental VR Application

How to start services and initialize VR headset:
- Start a screen
- Enter: ```Xorg -config ./xorg.conf.offscreen :10```
- Exit screen
- Start another screen
- Enter: ```sudo XRT_COMPOSITOR_FORCE_VK_DISPLAY=0 monado-service```
- Exit screen
- Enter: ```sudo XR_RUNTIME_JSON=/usr/local/share/openxr/1/openxr_monado.json DISPLAY=:10 ./openxr-example [args]```

The VR system should activate and the framebuffer should appear on the headsets internal displays.