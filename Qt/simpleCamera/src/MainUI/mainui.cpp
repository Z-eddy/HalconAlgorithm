#pragma execution_character_set("utf-8")
#include "mainui.h"

#include <QDebug>
#include <QThread>
#include <QTime>

#include "./ui_mainui.h"
#include "halconcpp/HalconCpp.h"
using namespace HalconCpp;

MainUI::MainUI(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainUI) {
  ui->setupUi(this);
}

MainUI::~MainUI() { delete ui; }

void MainUI::on_btnStart_clicked() {
  // Local iconic variables
  HObject ho_baseImg, ho_img, ho_grayImg, ho_Region;
  HObject ho_RegionFillUp, ho_ConnectedRegions, ho_SelectedRegions;

  // Local control variables
  HTuple hv_AcqHandle, hv_w, hv_h, hv_WindowHandle;
  HTuple hv_Numb;

  //关闭窗口
  // if (HDevWindowStack::IsOpen()) CloseWindow(HDevWindowStack::Pop());
  //打开相机,获得句柄
  OpenFramegrabber("DirectShow", 1, 1, 0, 0, 0, 0, "default", 8, "rgb", -1,
                   "false", "default", "0", -1, 1, &hv_AcqHandle);
  //捕捉一帧获得图像的大小
  GrabImage(&ho_baseImg, hv_AcqHandle);
  //获得图片的宽高
  GetImageSize(ho_baseImg, &hv_w, &hv_h);
  //新建窗口
  SetWindowAttr("background_color", "black");
  OpenWindow(0, 0, hv_w, hv_h, static_cast<Hlong>(ui->halconWindow->winId()),
             "visible", "", &hv_WindowHandle);
  HDevWindowStack::Push(hv_WindowHandle);
  //采集图像并处理
  //启动预抓图
  GrabImageStart(hv_AcqHandle, -1);
  QTime time;
  while (0 != 1) {
    qApp->processEvents();

    time.restart();
    //开始异步抓图
    GrabImageAsync(&ho_img, hv_AcqHandle, -1);
    //显示图像,其实不要这个也显示了
    if (HDevWindowStack::IsOpen())
      DispObj(ho_img, HDevWindowStack::GetActive());

    //开始处理图像
    //转化为灰度图
    Rgb1ToGray(ho_img, &ho_grayImg);
    //阈值提取亮处
    Threshold(ho_grayImg, &ho_Region, 200, 255);
    //填充区域
    FillUp(ho_Region, &ho_RegionFillUp);
    //找到连接的区域
    Connection(ho_RegionFillUp, &ho_ConnectedRegions);
    //选出大面积的目标
    SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "area", "and", 500,
                99999);
    if (HDevWindowStack::IsOpen()) {
      DispObj(ho_SelectedRegions, HDevWindowStack::GetActive());
    }
    //计算目标的个数
    CountObj(ho_SelectedRegions, &hv_Numb);
    //设置显示文字颜色
    if (HDevWindowStack::IsOpen())
      SetColor(HDevWindowStack::GetActive(), "white");
    //设置文字位置
    SetTposition(hv_WindowHandle, 24, 12);
    //查询可以设置的字体
    // query_font (WindowHandle, Font)
    //设置字体
    SetFont(hv_WindowHandle, "default-24");
    //设定输出文字
    // write_string会输出dev_set_color的颜色的字
    auto v{time.elapsed()};
    WriteString(hv_WindowHandle,
                "有" + hv_Numb + "个特别亮的目标" + "fps:" + 1000 / v);
    // disp_message (WindowHandle, '有'+Numb+'个目标啊', 'window', 12, 12,
    // 'yellow', 'false') 设置目标颜色为多彩色(12)
    if (HDevWindowStack::IsOpen()) SetColored(HDevWindowStack::GetActive(), 12);

    //每次处理完毕后停止,也可以设置断点的
    // stop ()
  }

  //采集结束,释放相机资源
  CloseFramegrabber(hv_AcqHandle);
}
