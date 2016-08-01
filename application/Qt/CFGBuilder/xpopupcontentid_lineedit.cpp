#include "xpopupcontentid_lineedit.h"
#include <QMouseEvent>
#include <QFileDialog>
#include <QIODevice>
#include <QCryptographicHash>
#include <QFile>
#include <QApplication>
#include <QClipboard>

const qint64 MaxLength_MD5 = 100;

XPopupContentId_LineEdit::XPopupContentId_LineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    this->setReadOnly(true);
}

void XPopupContentId_LineEdit::MidMd5(int position, int length)
{
    if(iMd5.length()>=(position+length))
        this->setText(QString(iMd5.mid(position, length)));
}

void XPopupContentId_LineEdit::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton)
    {
        // 弹出选择文件对话框，选择swf文件
        QString path = QFileDialog::getOpenFileName(this, tr("Open Swf File"),
                                         ipath, tr("swf (*.swf)"));

        // 打开swf文件，读取100byte数据做md5
        if(path.length()!=0)
        {
            ipath = path;
            QFile file(ipath);
            if(file.open(QIODevice::ReadOnly))
            {
                int namestart = ipath.lastIndexOf("/");
                int nameend = ipath.lastIndexOf(".");
                if(namestart!=-1 && nameend!=-1)
                {
                    QByteArray data;
                    data += ipath.mid(namestart+1, nameend-namestart-1);
                    data += file.read(MaxLength_MD5);
                    QByteArray md5 = QCryptographicHash::hash(data, QCryptographicHash::Md5);
                    iMd5 = md5.toHex();
                    this->setText(QString(iMd5.left(16)));
                    emit ContentChanged(false);
                }

                file.close();
            }
            
            QString tmp;
            tmp = ipath;
            tmp = tmp.mid(0, tmp.lastIndexOf("."));
            
            emit signalUpdatePath(tmp);
        }
    }
    else if(ev->button()==Qt::RightButton)
    {
        QClipboard* clipboard = QApplication::clipboard();
        clipboard->setText(this->text());
    }
}
