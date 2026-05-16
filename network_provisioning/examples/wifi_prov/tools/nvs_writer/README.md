nvs_writer — simple helper to write sec2 salt/verifier to NVS

This small standalone ESP-IDF app writes the test `salt` and `verifier` into NVS
namespace `sec2` with keys `salt` and `verifier`.

Build & flash (run from this directory):

```bash
# from tools/nvs_writer
idf.py build
idf.py -p /dev/tty.YOURPORT flash monitor
```

After running, the app commits the values to NVS and restarts the device. Then
flash the main provisioning app (or boot it) — it will read the blobs from NVS.

If you need different values, edit `main/app_main.c` in this folder and rebuild.
