# 来源与版权声明

本公开仓库是小智 v2.4.2 的板级定制派生代码，整合中文 CarCYD UI 和只读 VESC 遥测。组合代码遵循 GPL-3.0，原有许可和版权声明保留，不意味着原 MIT / Apache / OFL 组件变更了各自的许可。

## 组成

| 部分 | 来源 | 原许可 |
|---|---|---|
| 小智固件基础 | https://github.com/78/xiaozhi-esp32 | MIT，见 LICENSES/XiaoZhi-MIT.txt |
| CarCYD UI/core | https://github.com/muki01/Car-Dashboard-ESP32-CYD | GPL-3.0，见 LICENSE |
| MiniBike 皮肤设计参考 | https://github.com/Cyberj812/MiniBike-Stats | MIT，见 LICENSES/MiniBike-MIT.txt |
| Barlow 字体 | 原 CarCYD 字体工具目录 | SIL OFL，见 LICENSES/Barlow-OFL.txt |
| Noto Sans SC | Google Fonts / Noto 项目 | SIL OFL，见 LICENSES/NotoSansSC-OFL.txt |
| Material Design Icons | 原 CarCYD 字体工具目录 | Apache-2.0，见 LICENSES/MaterialDesignIcons-Apache-2.0.txt |
| S3 SIMD 汇编 | 锁定依赖 espressif/esp_lvgl_port | Apache-2.0，依赖源中保留 Espressif 声明 |

未修改的组件依赖由 ESP-IDF 组件管理器按 dependencies.lock 的版本和 component_hash 获取。完整离线源码归档的校验值记录于 docs/backup/firmware-checksums.json；大型可重建依赖缓存和本地运行资料未推送到 Git。

公开准备只修改仓库介绍、许可汇总、忽略规则和两个上游文档中的 AES 会话密钥示例（替换为零值示例）。另省略了六个生成字体文件头注释中的机器路径，字形数组及其余 C 代码未改变。固件功能代码与验收归档一致；没有导出设备的真实网络或会话凭据。
