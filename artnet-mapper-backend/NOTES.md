## Packer

```
$ sudo -E $(which packer) build 2039-Niagara.pkr.hcl
```

## Run on pi

normal

```
$ sudo EGL_PLATFORM=drm XDG_RUNTIME_DIR=/var/run/root/1000 /home/pi/build/1014-artnet-mapper/artnet-mapper-backend/bin/app -p 2039-Niagara
```

debug
```
$ sudo env "EGL_PLATFORM=drm " "XDG_RUNTIME_DIR=/var/run/root/1000" gdb --args /home/pi/build/1014-artnet-mapper/artnet-mapper-backend/bin/app -p 2039-Niagara
```