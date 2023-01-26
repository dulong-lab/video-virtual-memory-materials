# video-virtual-memory-materials
《关于编写 x64 Windows 10 驱动以了解虚拟内存这件事》系列视频附带的代码和材料。

© 杜龙实验室。保留所有权利。

软件、文档版权归原作者所有，提供链接仅为方便下载，如有侵权请立即与我们联系，我们将及时处理。

## 01. 简介
- 视频
  - [bilibili](https://www.bilibili.com/video/BV1cT411F7f4/)
  - [YouTube](https://youtu.be/Y6pdQGDOVA4)
- 材料
  - [PPT](Resources/01.pptx)
  - [虚拟地址空间 - Windows drivers | Microsoft Docs](https://docs.microsoft.com/zh-cn/windows-hardware/drivers/gettingstarted/virtual-address-spaces)

## 02. 配置驱动开发环境
- 视频
  - [bilibili](https://www.bilibili.com/video/BV1wY4y1n77F/)
  - [YouTube](https://youtu.be/ywMCQYVThj4)
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
  - [bilibili](https://www.bilibili.com/video/BV1FB4y1W7Z8/)
  - [YouTube](https://youtu.be/3moyxuQEOaQ)
- 材料
    - [用于项目和文件的模板 - Visual Studio (Windows) | Microsoft Docs](https://docs.microsoft.com/zh-cn/visualstudio/ide/creating-project-and-item-templates?view=vs-2022)
    - [Ntifs.h header - Windows drivers | Microsoft Docs](https://docs.microsoft.com/zh-cn/windows-hardware/drivers/ddi/ntifs/)
    - [Ntddk.h header - Windows drivers | Microsoft Docs](https://docs.microsoft.com/zh-cn/windows-hardware/drivers/ddi/ntddk/)
    - [Wdm.h header - Windows drivers | Microsoft Docs](https://docs.microsoft.com/zh-cn/windows-hardware/drivers/ddi/wdm/)
    - [once pragma | Microsoft Docs](https://docs.microsoft.com/zh-cn/cpp/preprocessor/once?view=msvc-170)
    - [Ntifs.h or ntddk.h ? — OSR](https://community.osr.com/discussion/283630/ntifs-h-or-ntddk-h)

## 04. 探索 CR4
- 视频
  - [bilibili](https://www.bilibili.com/video/BV1Ar4y1E7TH/)
  - [YouTube](https://youtu.be/IOmyMX6ZZy4)
- 材料
    - [Intel® 64 and IA-32 Architectures Software Developer Manuals ](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html)
        - [Intel® 64 and IA-32 Architectures Software Developer’s Manual Combined Volumes：1, 2A, 2B, 2C, 2D, 3A, 3B, 3C, 3D, and 4.pdf](Resources/Intel%C2%AE%2064%20and%20IA-32%20Architectures%20Software%20Developer%E2%80%99s%20Manual%20Combined%20Volumes%EF%BC%9A1,%202A,%202B,%202C,%202D,%203A,%203B,%203C,%203D,%20and%204.pdf)
            - SHA256: 105D78AB9C041D6BBC56EA422BD7D71CD57B8D94D89794666DB93707E68C5DCD

## 05. 探索页表
- 视频
  - [bilibili](https://www.bilibili.com/video/BV1vG411b7ep/)
  - [YouTube](https://youtu.be/-fUV4nj6M6o)
- 材料
  - [PPT](Resources/05.pptx)
  - [Visual Studio Code - Code Editing. Redefined](https://code.visualstudio.com/)
  - [D3.js - Data-Driven Documents](https://d3js.org/)
  - [be5invis/Sarasa-Gothic | 更纱黑体](https://github.com/be5invis/Sarasa-Gothic)
  
## 06. 手工翻译地址
- 视频
  - [bilibili](https://www.bilibili.com/video/BV1iv4y1F78U/)
  - [YouTube](https://youtu.be/eepRnrliDhY)

## 07. 编程翻译地址
- 视频
  - [bilibili](https://www.bilibili.com/video/BV1QN4y1G7zv/)
  - [YouTube](https://youtu.be/GeLei8KDfhk)
- 材料
  - [PPT](Resources/07.pptx)

## 08. 内存共享
- 视频
  - [bilibili](https://www.bilibili.com/video/BV1MN4y1c7nX/)
  - [YouTube](https://youtu.be/yrNGGFIE07c)
- 材料
  - [PPT](Resources/08.pptx)
  - [try-except statement (C) | Microsoft Docs](https://docs.microsoft.com/zh-cn/cpp/c-language/try-except-statement-c)
  - [Alex Ionescu on Twitter: "PatchGuard (and Mm) in RS5 will make short work of people messing around with PTE-based modifications thanks to MmCheckProcessShadow &amp; friends (MiCheckRelevantKernelShadows, etc). Looking forward to the lowering of the success of these types of attacks." / Twitter](https://twitter.com/aionescu/status/992260939574534144)

## 09. 自映射 前篇
- 视频
  - [bilibili](https://www.bilibili.com/video/BV1iY4y1T7F2/)
  - [YouTube](https://youtu.be/KHwzOlJu318)
- 材料
  - [PPT](Resources/09.pptx)

## 10. 自映射 后篇
- 视频
  - [bilibili](https://www.bilibili.com/video/BV1k14y1s7Aw/)
  - [YouTube](https://youtu.be/hrkJQ80wbSs)
- 材料
  - [PPT](Resources/10.pptx)
  - [Alex Ionescu on Twitter: "Redstone Build 14332 now adds MiRebaseDynamicRelocationRegions, and fully dynamic PTE/PDE/PXE base, with a call to SecureKernel/VSM." / Twitter](https://twitter.com/aionescu/status/725388108187852800)

## 11. 熔断
- 视频
  - [bilibili](https://www.bilibili.com/video/BV1qV4y1T7Gz/)
  - [YouTube](https://youtu.be/rRxzImUUWYc)
- 材料
  - [PPT](Resources/11.pptx)
  - [InSpectre.exe](https://www.grc.com/inspectre.htm)
  - [Meltdown and Spectre](https://meltdownattack.com/)
  - [Meltdown Paper](https://meltdownattack.com/meltdown.pdf)
      - [GitHub - IAIK/meltdown](https://github.com/IAIK/meltdown)
      - ["Meltdown"是什么？ - 知乎（论文中文翻译，第 6 小节不一样，可能不是最新）](https://zhuanlan.zhihu.com/p/33621030)
  - [Windows で Meltdown する - Qiita](https://qiita.com/msmania/items/794e3dbe82db86c4c4e6)
      - [GitHub - msmania/microarchitectural-attack](https://github.com/msmania/microarchitectural-attack)
  - [Black Hat USA 2018 - Meltdown Basics, Details, Consequences - YouTube](https://www.youtube.com/watch?v=Njgoed0lv0A)
  - [ADV180002 - 安全更新程序指南 - Microsoft - 用于缓解推理执行边信道漏洞的指南](https://msrc.microsoft.com/update-guide/zh-cn/vulnerability/ADV180002)

## 12. KVAS：Windows Meltdown 缓解
- 视频
  - [bilibili](https://www.bilibili.com/video/BV1pD4y1a7hP/)
  - [YouTube](https://youtu.be/CKTf2zUZuh4)
- 材料
  - [PPT](Resources/12.pptx)
  - [KVA Shadow: Mitigating Meltdown on Windows](https://msrc-blog.microsoft.com/2018/03/23/kva-shadow-mitigating-meltdown-on-windows/)
  - [Windows 10 KVAS and Software SMEP](https://wumb0.in/windows-10-kvas-and-software-smep.html)
  - [A Deep Dive Analysis of Microsoft’s Kernel Virtual Address Shadow Feature](https://www.fortinet.com/blog/threat-research/a-deep-dive-analysis-of-microsoft-s-kernel-virtual-address-shadow-feature)
  - [PCID & 与PTI的结合](http://happyseeker.github.io/kernel/2018/05/04/pti-and-pcid.html)

## 13. Total Meltdown
- 视频
  - [bilibili](https://www.bilibili.com/video/BV1LT411y739/)
  - [YouTube](https://youtu.be/aice3G30jUg)
- 材料
  - [PPT](Resources/13.pptx)
  - [CVE-2018-1038 - Microsoft](https://msrc.microsoft.com/en-US/security-guidance/advisory/CVE-2018-1038)
  - [Ulf Frisk on Twitter: "Total Meltdown? How the Windows 7 ...](https://twitter.com/ulffrisk/status/978657135130181632)
  - [Exploiting CVE-2018-1038 - Total Meltdown](https://blog.xpnsec.com/total-meltdown-cve-2018-1038/)
  - cn_windows_server_2008_r2_standard_enterprise_datacenter_and_web_with_sp1_x64_dvd_617598.iso
      - ed2k://|file|cn_windows_server_2008_r2_standard_enterprise_datacenter_and_web_with_sp1_x64_dvd_617598.iso|3368839168|D282F613A80C2F45FF23B79212A3CF67|/
  - [Microsoft® Update Catalog - KB4075211](https://www.catalog.update.microsoft.com/Search.aspx?q=KB4075211)
  - [aspxspy.aspx](https://github.com/tennc/webshell/blob/master/net-friend/aspx/aspxspy.aspx)
