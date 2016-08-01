#include "xpopuplineedit_dialog_label.h"
#include <QMessageBox>
#include <QMouseEvent>
#include <QApplication>
#include <QClipboard>

// 数字键总数
const int NumPad_Count = 23;
// 数字键Key的范围
static QRect NumPad_KeyPos[NumPad_Count] =
{
    {107, 382, 55, 24},   // L_FUNC
    {238, 380, 59, 26},   // R_FUNC
    {102, 411, 62, 24},   // CALL
    {239, 408, 52, 25},   // END
    {95, 439, 21, 55}, // MENU
    {288, 501, 17, 58}, // CLR
    {185, 401, 7, 13}, // LEFT
    {207, 401, 10, 13}, // RIGHT
    {192, 392, 17, 9}, // UP
    {190, 415, 20, 8}, // DOWN
    {193, 401, 12, 12}, // OK
    {175, 533, 53, 26}, // NUM0
    {118, 442, 53, 25}, // NUM1
    {174, 441, 54, 28}, // NUM2
    {230, 442, 54, 23}, // NUM3
    {118, 471, 54, 28}, // NUM4
    {175, 473, 55, 26}, // NUM5
    {230, 471, 56, 27}, // NUM6
    {118, 499, 54, 29}, // NUM7
    {173, 501, 54, 28}, // NUM8
    {231, 502, 54, 24}, // NUM9
    {118, 529, 53, 29}, // STAR
    {231, 534, 47, 20}  // WELL
};
// 数字键Key的名称
static QString NumPad_KeyName[NumPad_Count] =
{
    "L_FUNC", "R_FUNC",
    "CALL", "END",
    "MENU", "CLR",
    "LEFT", "RIGHT",
    "UP", "DOWN",
    "OK", "NUM0",
    "NUM1", "NUM2",
    "NUM3", "NUM4",
    "NUM5", "NUM6",
    "NUM7", "NUM8",
    "NUM9", "STAR",
    "WELL"
};
// 全键盘总数
const int FullPad_Count = 38;
// 全键盘Key的范围
static QRect FullPad_KeyPos[FullPad_Count] =
{
    {63, 307, 36, 30}, // L_FUNC
    {288, 306, 35, 30}, // R_FUNC
    {61, 338, 37, 31}, // CALL
    {291, 340, 32, 29}, // END
    {93, 324, 26, 29}, // MENU
    {157, 316, 14, 39}, // LEFT
    {217, 318, 10, 38}, // RIGHT
    {167, 309, 50, 8}, // UP
    {171, 356, 51, 9}, // DOWN
    {172, 320, 42, 35}, // OK
    {64, 374, 26, 31}, // Q
    {91, 373, 22, 33}, // W
    {116, 372, 24, 33}, // E
    {143, 374, 20, 31}, // R
    {168, 373, 23, 32}, // T
    {195, 374, 21, 32}, // Y
    {222, 374, 21, 31}, // U
    {247, 374, 25, 32}, // I
    {274, 374, 23, 32}, // O
    {300, 374, 23, 30}, // P
    {64, 409, 24, 31}, // A
    {90, 408, 24, 34}, // S
    {116, 410, 24, 32}, // D
    {143, 410, 21, 33}, // F
    {168, 411, 23, 31}, // G
    {195, 411, 23, 30}, // H
    {221, 411, 23, 30}, // J
    {248, 411, 23, 30}, // K
    {273, 411, 23, 30}, // L
    {299, 411, 23, 30}, // BS
    {64, 446, 24, 30}, // Z
    {91, 445, 23, 30}, // X
    {117, 447, 23, 30}, // C
    {142, 447, 23, 30}, // V
    {169, 447, 23, 30}, // B
    {196, 447, 23, 30}, // N
    {222, 447, 23, 30}, // M
    {144, 483, 97, 31}  // SPACE
};
// 全键盘Key的名称
static QString FullPad_KeyName[FullPad_Count] =
{
    "L_FUNC", "R_FUNC",
    "CALL", "END",
    "MENU", "LEFT",
    "RIGHT", "UP",
    "DOWN", "OK",
    "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P",
    "A", "S", "D", "F", "G", "H", "J", "K", "L", "BS",
    "Z", "X", "C", "V", "B", "N", "M", "SPACE"
};

XPopupLineEdit_Dialog_Label::XPopupLineEdit_Dialog_Label(QWidget *parent) :
    QLabel(parent)
{
    imode = NumPad;
    this->setMouseTracking(true);
}

void XPopupLineEdit_Dialog_Label::mouseMoveEvent(QMouseEvent *ev)
{
    if(imode==NumPad)   // 数字键盘样式
    {
        for(int i=0; i<NumPad_Count; i++)
        {
            if(NumPad_KeyPos[i].contains(ev->pos()))
            {
                this->setCursor(QCursor(Qt::PointingHandCursor));
                break;
            }
            else
                this->unsetCursor();
        }
    }
    else if(imode==FullPad) // 全键盘样式
    {
        for(int i=0; i<FullPad_Count; i++)
        {
            if(FullPad_KeyPos[i].contains(ev->pos()))
            {
                this->setCursor(QCursor(Qt::PointingHandCursor));
                break;
            }
            else
                this->unsetCursor();
        }
    }
}

void XPopupLineEdit_Dialog_Label::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton)    // 左键用于正常使用
    {
        if(imode==NumPad) // 数字键盘模式
        {
            for(int i=0; i<NumPad_Count; i++)
            {
                if(NumPad_KeyPos[i].contains(ev->pos()))
                {
                    emit ClickKey(NumPad_KeyName[i]);
                    break;
                }
            }
        }
        else if(imode==FullPad) // 全键盘模式
        {
            for(int i=0; i<FullPad_Count; i++)
            {
                if(FullPad_KeyPos[i].contains(ev->pos()))
                {
                    emit ClickKey(FullPad_KeyName[i]);
                    break;
                }
            }
        }
    }
    else if(ev->button()==Qt::RightButton)  // 右键用于获取按键区域
    {
        irect.setTopLeft(ev->pos());
    }
}

void XPopupLineEdit_Dialog_Label::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::RightButton)
    {
        irect.setBottomRight(ev->pos());

        // 弹出提示信息，显示区域范围
        QMessageBox* msg = new QMessageBox(this);
        QString copy;
        QString value;
        value += "(";
        QString value1;
        value1.setNum(irect.topLeft().x());
        value += value1;
        copy += value1;
        value += ",";
        copy += ",";
        value1.setNum(irect.topLeft().y());
        value += value1;
        copy += value1;
        value += ",";
        copy += ",";
        value1.setNum(irect.bottomRight().x()-irect.topLeft().x());
        value += value1;
        copy += value1;
        value += ",";
        copy += ",";
        value1.setNum(irect.bottomRight().y()-irect.topLeft().y());
        value += value1;
        copy += value1;
        value += ")";
        msg->setText(value);

        QClipboard* clipboard = QApplication::clipboard();
        clipboard->setText(copy);

        msg->exec();
    }
}
