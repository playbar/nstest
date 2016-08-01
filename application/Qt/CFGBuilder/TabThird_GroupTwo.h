/*
 * TabThird中的SoundInfo Group
 */
#ifndef TABTHIRD_GROUPTWO_H
#define TABTHIRD_GROUPTWO_H

// declearation
class QLabel;
class QComboBox;
class QLineEdit;
class QTextCodec;
class QGridLayout;
class XEasyXml;
class QSpinBox;

// include
#include <QGroupBox>

class TabThird_GroupTwo : public QGroupBox
{
public:
    TabThird_GroupTwo(QWidget* parent = NULL);

private:
    // 构造Group
    void SetupGroup();

    // 是否静音
    void SetupMute();

    // 音量大小
    void SetupVolume();

    // 声音输出设备
    void SetupSounddevice();

public:
    // 恢复默认
    void Default();

    // 读取数据
    void ReadData(XEasyXml* xml);

    // 保存数据
    void SaveData(XEasyXml* xml);

private:
    // 是否静音
    QLabel*     imute;
    QComboBox*  imute_list;

    // 音量大小
    QLabel*     ivolume;
//    QSpinBox*  ivolume_box;
    QLineEdit* ivolume_input;

    // 声音输出设备
    QLabel*     isounddevice;
    QComboBox*  isounddevice_list;

    // 排版(4, 2)
    QGridLayout*    iLayout;
};

#endif // TABTHIRD_GROUPTWO_H
