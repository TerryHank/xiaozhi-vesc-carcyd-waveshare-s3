# 小智 AI + VESC 中文智能仪表

面向 **Waveshare ESP32-S3-Touch-LCD-3.5（非 B 型）** 的已验收源码快照。一块 ESP32-S3 同时运行小智语音助手、只读 VESC 遥测和中文 CarCYD 界面。

## 功能

- 原版 CarCYD 圆环、六表、侧边导航、数据曲线、故障和设置页；另有四种按 MiniBike 设计文档实现的皮肤。
- 原生横屏 **480×320**、竖屏 **320×480**，IMU 四方向旋转，触摸和摄像头方向同步适配。
- 小智语音对话、完整中文字库、聊天历史滑动、新消息自动到底部和拍照识图。
- 触屏选择 Wi-Fi、屏幕键盘输入密码；长按 BOOT 进入配网，短按 BOOT 返回。
- 闲置不自动暗屏或关机；保留手动 PWR。
- VESC 缓存遥测与 MCP 查询共用数据：电压、电流、功率、电气转速、温度和故障等。
- PSRAM 直连 DMA、S3 SIMD、局部刷新、减少聊天遮挡重绘和重复排版，并修复已观察到的 LVGL 栈溢出。

VESC 通讯使用 **UART1 TX=GPIO43 / RX=GPIO44**，默认 115200，可在设置页修改。控制器目标为 MKSESC B100300 G2 / VESC 6.2。该快照验收时尚未连接控制器，真实 VESC 通讯和车速标定仍待接线验证；轮径、极对数、传动比未知时不推测车速。没有油门、制动或修改电调参数的命令。

## 构建

使用 **ESP-IDF 6.0.2**。保留了实际构建的 `sdkconfig`、板级 `config.json` 和 `dependencies.lock`；LVGL 锁定在 9.5.0。依赖由组件管理器按锁文件获取，`managed_components/` 为可重建依赖缓存，不纳入本仓库。

```sh
git clone https://github.com/TerryHank/xiaozhi-vesc-carcyd-waveshare-s3.git
cd xiaozhi-vesc-carcyd-waveshare-s3

# 激活 ESP-IDF 6.0.2 环境后
idf.py --version
python scripts/build.py waveshare/esp32-s3-touch-lcd-3.5 --name esp32-s3-touch-lcd-3.5-carcyd --language zh-CN --wake-word nihaoxiaozhi
```

首次烧录可使用 `idf.py -p PORT flash`，将 `PORT` 换成实际串口。更新已有同分区设备时可只写应用区以保留网络配置：

```sh
python -m esptool --chip esp32s3 --port PORT --baud 921600 write-flash 0x20000 build/xiaozhi.bin
```

字体已生成并包含在源码中。需要重新生成时，在 `scripts/carcyd_fonts/` 执行 `npm install`，再在工程根目录执行 `python scripts/generate_carcyd_fonts.py`。

主要适配目录：`main/boards/waveshare/esp32-s3-touch-lcd-3.5/`。公共 SPI 显示构造器的新增参数有默认值，其他调用保留原有行为。

## 验证与性能

63 项主机测试、1480 组设备端 SIMD 像素/步幅/对齐/边界测试，以及实际烧录校验通过。设备使用者确认显示、旋转触摸、语音、识图和配网正常。

同一设备的固定场景基准，每项预热约 1 秒、采样约 5 秒：

| 场景 | 首轮 80 MHz 版本 | 本快照采用的方案 |
|---|---:|---:|
| 纯色全屏重绘 | 18.41 FPS | 26.18 FPS |
| CarCYD 圆环 | 45.51 FPS | 58.76 FPS |
| 独立聊天页滚动 | 8.68 FPS | 19.28 FPS |
| 仪表上的聊天浮层滚动 | 3.05 FPS | 17.14 FPS |

这些是固定场景的渲染周期测量，不代表所有页面恒定 60 FPS。更改字节顺序的对照方案虽达到 31.11 FPS 纯色全屏，但实际圆环与聊天更慢，因此未用于本快照。结构化数据见 [benchmark-results.json](docs/backup/benchmark-results.json)。临时自动基准已从正式源码移除。

对应的已烧录应用 SHA-256：

```text
99283478b630eea3b6568936f3bc021c1bef51e0892ec0d913027fc1d4bc37c7
```

构建产物的记录见 [firmware-checksums.json](docs/backup/firmware-checksums.json)。这里只备份代码与构建资料，不上传整片 Flash、NVS、个人 Wi-Fi 配置、运行日志或调试内存转储。

## 来源与许可

本项目包含 CarCYD 的 GPL-3.0 代码，组合派生代码按 **GPL-3.0** 分发；各组件和素材原有许可继续保留。详见 [LICENSE](LICENSE)、[NOTICE.md](NOTICE.md) 和 [LICENSES](LICENSES/)。

- 小智基础：[78/xiaozhi-esp32](https://github.com/78/xiaozhi-esp32)，本快照基于 v2.4.2，原有 MIT 许可保留。
- CarCYD：[muki01/Car-Dashboard-ESP32-CYD](https://github.com/muki01/Car-Dashboard-ESP32-CYD)，参考提交 `a87aa340da64a05fbe4e42f36ebc16fdfac8b8db`，GPL-3.0。
- MiniBike：[Cyberj812/MiniBike-Stats](https://github.com/Cyberj812/MiniBike-Stats)，参考提交 `5e12db8fd62f462579fbd7398af0247a67055f72`，MIT；四种皮肤按设计文档实现，未声称复用了其占位式串口渲染作为图形渲染。
- Barlow / Noto Sans SC：SIL OFL；Material Design Icons：Apache-2.0。
- SIMD 汇编来自锁定版本的 Espressif `esp_lvgl_port`，Apache-2.0；本仓库添加与 LVGL 9.5 实际类型匹配的接口适配。

原始英文说明保存在 [README.upstream.md](README.upstream.md)。上游 GitHub 元数据和多板 CI 脚本作为文本参考保存在 `docs/upstream/github/`，本次推送不触发上游全板构建流程。
