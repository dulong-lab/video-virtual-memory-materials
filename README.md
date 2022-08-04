# video-virtual-memory-materials
《关于编写 x64 Windows 10 驱动以了解虚拟内存这件事》系列视频附带的代码和材料。

© 杜龙实验室。保留所有权利。

软件、文档版权归原作者所有，提供链接仅为方便下载，如有侵权请立即与我们联系，我们将及时处理。

## 01. 简介
- 视频
  -  [bilibili](https://www.bilibili.com/video/BV1Ar4y1g7DX/)
- 材料
  - [PPT](Resources/01.pptx)
  - [虚拟地址空间 - Windows drivers | Microsoft Docs](https://docs.microsoft.com/zh-cn/windows-hardware/drivers/gettingstarted/virtual-address-spaces)

## 02. 配置驱动开发环境
- 视频
  -  [bilibili](https://www.bilibili.com/video/BV1wY4y1n77F/)
- 材料
    - [开发工具下载页面（Visual Studio、SDK、WDK）](https://docs.microsoft.com/zh-cn/windows-hardware/drivers/download-the-wdk)
    - [虚拟机下载页面（Virtual Box）](https://www.virtualbox.org/wiki/Downloads)
        - [通过天翼云盘下载（VirtualBox-6.1.34a-150636-Win.exe）](https://cloud.189.cn/web/share?code=n2mUJnueENfa) 访问码：oah3
            - SHA256: 7FD356F17EAD9AA6B491C3B8A2A52A08B2EB1B84854A2150E23B85C25E315612
    - [Windows 10 镜像下载工具下载页面](https://www.microsoft.com/zh-cn/software-download/windows10)
        - [通过天翼云盘下载（Windows10.0.19041.1）](https://cloud.189.cn/web/share?code=Mv6VF3JfYzqu) 访问码：pwb7
            - SHA256: 856ED08830A28F273A94C6A80D396FE221276A5F06D44B926BB8BC53AD7F03C1
    - [KmdKit by Four-F](http://four-f.narod.ru/)
        - [通过 Wayback Machine 下载（KmdKit.zip）](https://web.archive.org/web/20070327162529/http://www.freewebs.com/four-f/KmdKit/KmdKit.zip)
        - [KmdKit.zip](Resources/KmdKit.zip)
            - SHA256: 08B4E2AC1006324AFA1936AF3436EAC2411E6CF1C6D4057A2A3790B82C468912

## 03. 创建驱动模板
- 视频
  -  [bilibili](https://www.bilibili.com/video/BV1FB4y1W7Z8/)
- 材料
    - [用于项目和文件的模板 - Visual Studio (Windows) | Microsoft Docs](https://docs.microsoft.com/zh-cn/visualstudio/ide/creating-project-and-item-templates?view=vs-2022)
    - [Ntifs.h header - Windows drivers | Microsoft Docs](https://docs.microsoft.com/zh-cn/windows-hardware/drivers/ddi/ntifs/)
    - [Ntddk.h header - Windows drivers | Microsoft Docs](https://docs.microsoft.com/zh-cn/windows-hardware/drivers/ddi/ntddk/)
    - [Wdm.h header - Windows drivers | Microsoft Docs](https://docs.microsoft.com/zh-cn/windows-hardware/drivers/ddi/wdm/)
    - [once pragma | Microsoft Docs](https://docs.microsoft.com/zh-cn/cpp/preprocessor/once?view=msvc-170)
    - [Ntifs.h or ntddk.h ? — OSR](https://community.osr.com/discussion/283630/ntifs-h-or-ntddk-h)

## 04. 探索 CR4
- 视频
  -  [bilibili](https://www.bilibili.com/video/BV1Ar4y1E7TH/)
- 材料
    - [Intel® 64 and IA-32 Architectures Software Developer Manuals ](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html)
        - [Intel® 64 and IA-32 Architectures Software Developer’s Manual Combined Volumes：1, 2A, 2B, 2C, 2D, 3A, 3B, 3C, 3D, and 4.pdf](Resources/Intel%C2%AE%2064%20and%20IA-32%20Architectures%20Software%20Developer%E2%80%99s%20Manual%20Combined%20Volumes%EF%BC%9A1,%202A,%202B,%202C,%202D,%203A,%203B,%203C,%203D,%20and%204.pdf)
            - SHA256: 105D78AB9C041D6BBC56EA422BD7D71CD57B8D94D89794666DB93707E68C5DCD

## 05. 探索页表
- 视频
  -  [bilibili](https://www.bilibili.com/video/BV1vG411b7ep/)
- 材料
  - [PPT](Resources/05.pptx)
  - [Visual Studio Code - Code Editing. Redefined](https://code.visualstudio.com/)
  - [D3.js - Data-Driven Documents](https://d3js.org/)
  - [be5invis/Sarasa-Gothic | 更纱黑体](https://github.com/be5invis/Sarasa-Gothic)
  
## 06. 探索进程对象
## 07. 映射内存
## 08. 写时复制
## 09. 保护数据
## 10. 熔断
