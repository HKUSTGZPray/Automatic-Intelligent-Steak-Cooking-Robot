# Automatic-Intelligent-Steak-Cooking-Robot

## 1 主要功能：

我们机器人的整体工作流程为：

1. 远程控制：由操作者将牛排放入加热框中，使用手机或操控面板进行挡位选择，机器人开始工作。
2. 放入牛排：将牛排放入机器内，加热框调整至水平状态，等待加热模块的温度达到事前选择的设定温度。
3. 注入食用油：待温度达到后，蠕动泵将从侧面的盛油罐内抽油滴在牛排上。
4. 煎制牛排：待加油步骤结束，牛排翻面机构运行将牛排翻面，并水平运送至加热模块正上方开始煎制。一段时间后（与事前选择的牛排熟度有关），加热框载着牛排退回前端进行一次新的加油、翻面操作。之后再次运送至加热模块正上方煎制牛排的另一面。煎制结束后，加热框回到前端，开口朝下，牛排自动滑入餐盘内，煎制机器人工作完成。

针对牛排煎制的特点以及本作品的功能，牛排煎制机器人所要实现的功能如下：

1. 自动化：能够全自动高效完成牛排的煎制。且实现机电一体化，并存在实时反馈的调控系统。
2. 智能性：手机APP连接，远程智能操控。挡位选择多，人性化制作满足不同用户的口味需求。
3. 安全性：机器上和手机APP都设有紧急断电按钮，确保用户的使用的安全。

## 2 设计方案：

本作品主要通过三台步进电机、两根丝杠滑台以及一台蠕动泵地控制，完成牛排地整个煎制过程。作品整体结构尺寸长×宽×高=750mm×480mm×400mm，总质量12kg，由牛排加热装置、牛排固定翻转运动系统、加油加料装置、排气装置以及控制电路与远程控制系统五部分组成。具体三维结构如图2-1所示：

![](https://github.com/HKUSTGZPray/Automatic-Intelligent-Steak-Cooking-Robot/blob/main/images/%E6%88%AA%E5%B1%8F2024-06-06%2021.33.28.jpg)

<div align = "center">图2-1 作品三维结构图</div>

其整体的工作流程图如图2-2所示：

![](https://github.com/HKUSTGZPray/Automatic-Intelligent-Steak-Cooking-Robot/blob/main/images/d41bb068558a5017d251c003d557793e.png)

<div align = "center">图2-2 工作流程图</div>

### 2.1 牛排加热装置

牛排加热装置，是牛排煎制机最基础的功能。我们在设计上并未选择全新的方案，而是直接参考了市面上已有的几种方案，分别是电磁感应式加热以及电热丝加热，他们的对比见表2-1所示：

<div align = "center">表2-1 牛排加热方案对比</div>

| 方案                  | 优点                                           | 缺点                                   |
| ---------------------- | ---------------------------------------------- | -------------------------------------- |
| 方案一：电磁感应式加热 | 加热速度快；使用方便；操作简单；环保节能       | 成本较高；感应部件的互换性和适应性较差 |
| 方案二：电热丝加热     | 耐高温；加热均匀；使用简单；寿命长；功率偏差小 | 热量的损失浪费大；保温难               |

经对比，我们选择的是电热丝加热这种方案，具体实物如图2-3所示：为实时监测温度，我们在电阻丝一侧固定了两个耐高温的温度传感器，通过传感器反馈的温度控制加热温度的变化以保证所需加热温度的精度。

![](https://github.com/HKUSTGZPray/Automatic-Intelligent-Steak-Cooking-Robot/blob/main/images/20bb2a4b8070387d20a8dbbf3f18dd14.jpeg)

<div align = "center">图2-3 固定好的两根温度传感器</div>

### 2.2 牛排固定翻转运动系统

#### 2.2.1 总体结构

牛排固定翻转运动系统，是我们整个设计作品的核心部分。我们根据整体装置的设计配合，提出了两套整体固定与运动方案，分别是双驱动丝杠滑轨平移原理方案和单驱动滑轨吊杆平移原理方案。他们的对比见表2-2所示：

<div align = "center">表2-2 牛排加热方案对比</div>

| 方案              | 优点              | 缺点                                              |
| ------------------ | ------------------ | -------------------------------------------------- |
| 双驱动丝杠滑轨平移 | 稳定；节约立体空间 | 成本较高；增加作品重量                             |
| 单驱动滑轨吊杆平移 | 成本更低；美观     | 相对没有那么稳定；浪费立体空间；后续配合机构更复杂 |

考虑到机构的稳定，并且尽量减小作品的体积，我们最终选择了双驱动丝杠滑轨平移原理的方案。

其主要由以下三个部分组成：x轴旋转机构、加热槽装置和水平运动机构。

#### 2.2.2 x轴旋转机构

x轴旋转，我们定义为牛排的旋转，其充分模拟了用户日常煎制牛排的翻面步骤。如图2-4所示，采用了将联轴器与加热槽通过法兰联结的方式穿过两端的L型件固定在轴承支架上，并用一台步进电机带动进行旋转。其优点在于易拆卸，且加热效果好。

![](https://github.com/HKUSTGZPray/Automatic-Intelligent-Steak-Cooking-Robot/blob/main/images/bdab887f123906700e3b639b9e79e5ca.jpeg)

<div align = "center">图2-4 旋转连接机构</div>

为保证加热时刻的水平状态，我们在加热槽的两侧添加了激光对射光电感应开关，其简易原理是：当加热槽旋转到水平角度后，金属框阻挡感应开关发射的激光使得旋转运动停止。

#### 2.2.3 加热槽装置

关于牛排的固定，我们主要考虑过两种方案，分别是机械夹夹取和加热框放置，其优缺点见表2-3所示：

 <div align = "center">表2-3 牛排加热槽方案对比</div>

| 方案      | 优点          | 缺点              |
|------------|----------------|--------------------|
| 机械夹夹取 | 牛排加热效果好 | 翻转复杂且夹紧困难 |
| 加热框放置 | 翻转简单       | 空间占用大         |

经过综合对比，我们最终采用了第二种加热槽放置的方案。其构成主要由定制金属框以及金属网等部分组成。其结构的造型设计见图2-5所示。

![](https://github.com/HKUSTGZPray/Automatic-Intelligent-Steak-Cooking-Robot/blob/main/images/20ab6757ac141ee3e30181fcc8017ac2.jpeg)

<div align = "center">图2-5 金属加热框</div>

#### 2.2.4水平运动机构

如图2-6所示，该机构我们采用了两台丝杠同时运作传动的方式，将加热槽与旋转机构通过L型件固定在丝杠滑块上，丝杠行程为400mm。

![](https://github.com/HKUSTGZPray/Automatic-Intelligent-Steak-Cooking-Robot/blob/main/images/0d10bdc724798f74f73c995a19e0374c.jpeg)

<div align = "center">图2-6 丝杠滑台</div>

对于行程的反馈，我们提出了以下几个方案（见表2-4所示）：

<div align = "center">表2-4 行程反馈方案对比 </div>

| 方案            | 优点          | 缺点                  |
| ---------------- | -------------- | ---------------------- |
| 金属感应接近开关 | 反应快；精度高 | 易受周围物体影响       |
| 直接程序设计     | 方法简单       | 存在偏差且不宜矫正     |
| 超声波测距传感器 | 可实时反馈数据 | 精度不高；容易受到干扰 |

经过对比，在本作品中，由于周围环境稳定，我们选择了性价比与功能性强的金属感应接近开关作为行程反馈的控制方案。（如图2-7所示）

![](https://github.com/HKUSTGZPray/Automatic-Intelligent-Steak-Cooking-Robot/blob/main/images/79d9b176af51a57b39eb059126fcd5f4.jpeg)

<div align = "center">图2-7 金属感应接近开关通过步进电机的靠近判断运动的停止</div>

### 2.3加油加料装置

针对市场上牛排煎制设备无法在煎制过程中添加食用油的缺陷，我们创新引入了加油加料装置。如图2-8所示，主要采用蠕动泵驱动的原理，利用蠕动泵、液体管道以及喷头实现牛排煎制过程中的自动加油加料，进一步提高煎制过程自动化程度。

![](https://github.com/HKUSTGZPray/Automatic-Intelligent-Steak-Cooking-Robot/blob/main/images/2321467cc0ffc9405b5ba45318f3b1c2.jpeg)

<div align = "center">图2-8 蠕动泵</div>

### 2.4排气装置

经过初期的多次试验，我们发现整体机箱存在发热严重的问题，虽然不影响整体功能的实现。为确保整部作品的性能不受影响以及用户的使用安全，我们在加热装置的后上方特殊添加了一台排油烟降温风扇，如图2-9所示。

![](https://github.com/HKUSTGZPray/Automatic-Intelligent-Steak-Cooking-Robot/blob/main/images/b7ad1f17b089ce5ea5535a8130c5eb3b.jpeg)

<div align = "center">图2-9 后盖的排气装置</div>

### 2.5控制电路与远程控制系统

本系统是一个基于Arduino UNO(以下简称Arduino)开发板的机械控制电路。系统由多个模块组成，分别是Arduino控制板，12V开关电源，A4988步进电机驱动模块，DM542步进电机驱动模块，LM2596降压模块，接近开关，光电开关，温度控制模块，蜂鸣器模块以及继电器模块。系统可直接接入市网电压，通过开关电源以及LM2596降压模块实现12V直流供电和5V直流供电。以接近开关和光电开关作为检测模块，实现加热框的位置归零操作；以PT100温度探头检测加热温度。单片机采集传感器的反馈数据，进行分析和处理。通过单片机控制步进电机驱动模块来驱动步进电机的运动，实现机器的煎制操作。其控制电路如图2-10所示。

![](https://github.com/HKUSTGZPray/Automatic-Intelligent-Steak-Cooking-Robot/blob/main/images/e56c2b774d273230f05b174b174f8fb4.jpeg)

<div align = "center">图2-10 本作品的控制电路部分</div>

## 3 工作原理及性能分析：

### 3.1 工作原理

#### 3.1.1 蠕动泵工作原理

蠕动泵的整体架构就是由一颗中心滚轮夹挤一根充满流体的软管，随着滚轮的转动，管内流体向前移动。滚轮侧面装有若干根转棍，随着滚轮一齐转动，管内形成负压，液体随之流动。

在我们的机器人中，通过步进电机带动蠕动泵转动完成抽油工作。其优点主要在于食品安全性高，密封性好，稳定性好且维护简单。

#### 3.1.2 牛排翻面原理

考虑到完全模拟现实人工煎制牛排的步骤与处理方式以达到最佳的食用效果。我们放弃了市面上大部分牛排机选择的双面加热方式，而是采用了一个加热面贴合牛排加热框+翻面的方式完成牛排煎制。

其主要工作原理是将牛排放置于加热框内，其通过两根自制金属细轴向外连接步进电机完成翻面。

为解决由于翻面会产生的加热框触碰问题，我们向前设计了一个延展空间用于提供翻面所需要的空间并通过丝杆滑台运动的方式将其运送过去。
其优点在于提供足够的空间使得牛排与油能够很好的接触并且加热煎制。且整体机构稳定，自动化程度高。

### 3.2 性能分析

本作品全称为全自动智能牛排煎制机器人，可以实现完全的自动煎牛排功能，同时兼具对牛排熟度的挡位选择、加油加料等功能。整体流程在十分钟左右，与人工煎制牛排时间大致相等。而煎制出的牛排口感与人工煎制也基本一致，其性能已足够适用于日常生活中。

经过设计与分析，从原理设计到作品样机进行了制作，作品实物外形照片见图3-1所示，煎制完成的牛排如图3-2所示。

![](https://github.com/HKUSTGZPray/Automatic-Intelligent-Steak-Cooking-Robot/blob/main/images/6dfcfa4f3e7e3d9217c93d8f9c96fa54.jpeg)

<div align = "center">图3-1 牛排煎制机实物图</div>

![](https://github.com/HKUSTGZPray/Automatic-Intelligent-Steak-Cooking-Robot/blob/main/images/e8e57598b7129249720a4c76efe0d58e.jpeg)

<div align = "center">图3-2 煎制完成的牛排</div>
