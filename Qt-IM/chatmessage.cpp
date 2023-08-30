#include "chatmessage.h"
#include <QFontMetrics>
#include <QPaintEvent>
#include <QDateTime>
#include <QPainter>
#include <QMovie>
#include <QLabel>
#include <QDebug>

QNChatMessage::QNChatMessage(int laid, int taid, QWidget *parent) : QWidget(parent)
{
    //设置字体样式
    QFont te_font = this->font();
    te_font.setPointSize(13);
    this->setFont(te_font);
    //设置聊天双方头像
    m_leftPixmap = QPixmap(QString(":/res/avatar%1.png").arg(taid));
    m_rightPixmap = QPixmap(QString(":/res/avatar%1.png").arg(laid));
}

void QNChatMessage::setTextSuccess()
{

    m_isSending = true;
}


    //聊天信息设置
void QNChatMessage::setText(QString text, QString time, QSize allSize, QNChatMessage::User_Type userType)
{
    m_msg = text;
    m_userType = userType;
    m_time = time;
    m_curTime = QDateTime::fromTime_t(time.toInt()).toString("hh:mm");
    m_allSize = allSize;
    //消息重绘操作
    this->update();
}

    //计算消息文本控件大小
QSize QNChatMessage::getRealString(QString src)
{
    QFontMetricsF fm(this->font());
    //计算行高
    m_lineHeight = fm.lineSpacing();
    //统计换行符的个数
    int nCount = src.count("\n");

    int nMaxWidth = 0;
    //没有换行符的情况下计算消息文本的宽度
    if(nCount == 0) {
        nMaxWidth = fm.width(src);
        QString value = src;

        //超过最大宽度进行切分
        if(nMaxWidth > m_textWidth) {
            nMaxWidth = m_textWidth;
            QString temp = "";
            int num = fm.width(value) / m_textWidth;
            num = ( fm.width(value) ) / m_textWidth;
            nCount += num;
            int size = m_textWidth / fm.width(" ");
            int ttmp = num*fm.width(" ");
            for(int i = 0; i < num; i++) {
                temp += value.mid(i*size, (i+1)*size) + "\n";
            }
            src.replace(value, temp);
        }
    }
    //有换行符
    else {
        for(int i = 0; i < (nCount + 1); i++) {
            QString value = src.split("\n").at(i);
            nMaxWidth = fm.width(value) > nMaxWidth ? fm.width(value) : nMaxWidth;
            if(fm.width(value) > m_textWidth) {
                nMaxWidth = m_textWidth;
                QString temp = "";
                int num = fm.width(value) / m_textWidth;
                num = ((i+num)*fm.width(" ") + fm.width(value)) / m_textWidth;
                nCount += num;
                int size = m_textWidth / fm.width(" ");
                for(int i = 0; i < num; i++) {
                    temp += value.mid(i*size, (i+1)*size) + "\n";
                }
                src.replace(value, temp);
            }
        }
    }
    return QSize(nMaxWidth+m_spaceWid+1, (nCount + 1) * m_lineHeight+2*m_lineHeight+1);
}

    //计算消息控件中各个元素的位置和大小
QSize QNChatMessage::fontRect(QString str)
{
    m_msg = str;
    int minHei = 30;
    int iconWH = 40;
    int iconSpaceW = 20;
    int iconRectW = 5;
    int iconTMPH = 10;
    int sanJiaoW = 6;
    int kuangTMP = 20;
    int textSpaceRect = 12;

    // 计算消息框的宽度和文本区域的宽度
    m_kuangWidth = this->width() - kuangTMP - 2*(iconWH+iconSpaceW+iconRectW);
    m_textWidth = m_kuangWidth - 2*textSpaceRect;
    m_spaceWid = this->width() - m_textWidth;

    // 设置左侧头像和右侧头像的位置和大小
    m_iconLeftRect = QRect(iconSpaceW, iconTMPH, iconWH, iconWH);
    m_iconRightRect = QRect(this->width() - iconSpaceW - iconWH-25, iconTMPH, iconWH, iconWH);

    // 计算整个消息控件的大小
    QSize size = getRealString(m_msg);

    // 计算消息控件的高度
    int hei = size.height() < minHei ? minHei : size.height();

    // 设置三角形的位置和大小
    m_sanjiaoLeftRect = QRect(iconWH+iconSpaceW+iconRectW, m_lineHeight/2, sanJiaoW, hei - m_lineHeight);
    m_sanjiaoRightRect = QRect(this->width() - iconRectW - iconWH - iconSpaceW - sanJiaoW-25, m_lineHeight/2, sanJiaoW, hei - m_lineHeight);


    // 根据文本宽度和空白宽度判断消息框的位置和大小
    if(size.width() < (m_textWidth+m_spaceWid)) {
        m_kuangLeftRect.setRect(m_sanjiaoLeftRect.x()+m_sanjiaoLeftRect.width(), m_lineHeight/4*3, size.width()-m_spaceWid+2*textSpaceRect, hei-m_lineHeight);
        m_kuangRightRect.setRect(this->width() - size.width() + m_spaceWid - 2*textSpaceRect - iconWH - iconSpaceW - iconRectW - sanJiaoW-25,
                                 m_lineHeight/4*3, size.width()-m_spaceWid+2*textSpaceRect, hei-m_lineHeight);
    } else {
        m_kuangLeftRect.setRect(m_sanjiaoLeftRect.x()+m_sanjiaoLeftRect.width(), m_lineHeight/4*3, m_kuangWidth, hei-m_lineHeight);
        m_kuangRightRect.setRect(iconWH + kuangTMP + iconSpaceW + iconRectW - sanJiaoW-25, m_lineHeight/4*3, m_kuangWidth, hei-m_lineHeight);
    }
    m_textLeftRect.setRect(m_kuangLeftRect.x()+textSpaceRect,m_kuangLeftRect.y()+iconTMPH,
                           m_kuangLeftRect.width()-2*textSpaceRect,m_kuangLeftRect.height()-2*iconTMPH);
    m_textRightRect.setRect(m_kuangRightRect.x()+textSpaceRect,m_kuangRightRect.y()+iconTMPH,
                            m_kuangRightRect.width()-2*textSpaceRect,m_kuangRightRect.height()-2*iconTMPH);

    return QSize(size.width(), hei);
}

    //绘制消息控件的外观
void QNChatMessage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
   // 设置绘制的渲染提示，消除锯齿
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::gray));
   // 用户
    if(m_userType == User_Type::User_She) {
        //头像
        painter.drawPixmap(m_iconLeftRect, m_leftPixmap);

        //框加边
        QColor col_KuangB(234, 234, 234);
        painter.setBrush(QBrush(col_KuangB));
        painter.drawRoundedRect(m_kuangLeftRect.x()-1,m_kuangLeftRect.y()-1,m_kuangLeftRect.width()+2,m_kuangLeftRect.height()+2,4,4);
        //框
        QColor col_Kuang(255,255,255);
        painter.setBrush(QBrush(col_Kuang));
        painter.drawRoundedRect(m_kuangLeftRect,4,4);

        //三角
        QPointF points[3] = {
            QPointF(m_sanjiaoLeftRect.x(), 35),
            QPointF(m_sanjiaoLeftRect.x()+m_sanjiaoLeftRect.width(), 30),
            QPointF(m_sanjiaoLeftRect.x()+m_sanjiaoLeftRect.width(), 40),
        };
        QPen pen;
        pen.setColor(col_Kuang);
        painter.setPen(pen);
        painter.drawPolygon(points, 3);

        //三角加边
        QPen penSanJiaoBian;
        penSanJiaoBian.setColor(col_KuangB);
        painter.setPen(penSanJiaoBian);
        painter.drawLine(QPointF(m_sanjiaoLeftRect.x() - 1, 30), QPointF(m_sanjiaoLeftRect.x()+m_sanjiaoLeftRect.width(), 24));
        painter.drawLine(QPointF(m_sanjiaoLeftRect.x() - 1, 30), QPointF(m_sanjiaoLeftRect.x()+m_sanjiaoLeftRect.width(), 36));

        //内容
        QPen penText;
        penText.setColor(QColor(51,51,51));
        painter.setPen(penText);
        QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
        option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        painter.setFont(this->font());
        painter.drawText(m_textLeftRect, m_msg,option);
    }

    // 自己
    else if(m_userType == User_Type::User_Me) {

        painter.drawPixmap(m_iconRightRect, m_rightPixmap);

        //框
        QColor col_Kuang(75,164,242);
        painter.setBrush(QBrush(col_Kuang));
        painter.drawRoundedRect(m_kuangRightRect,4,4);

        //三角
        QPointF points[3] = {
            QPointF(m_sanjiaoRightRect.x()+m_sanjiaoRightRect.width(), 35),
            QPointF(m_sanjiaoRightRect.x(), 30),
            QPointF(m_sanjiaoRightRect.x(), 40),
        };
        QPen pen;
        pen.setColor(col_Kuang);
        painter.setPen(pen);
        painter.drawPolygon(points, 3);

        //内容
        QPen penText;
        penText.setColor(Qt::white);
        painter.setPen(penText);
        QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
        option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        painter.setFont(this->font());
        painter.drawText(m_textRightRect,m_msg,option);
    }
    else if(m_userType == User_Type::User_Time) { // 时间
        QPen penText;
        penText.setColor(QColor(153,153,153));
        painter.setPen(penText);
        QTextOption option(Qt::AlignCenter);
        option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        QFont te_font = this->font();
        te_font.setFamily("MicrosoftYaHei");
        te_font.setPointSize(10);
        painter.setFont(te_font);
        painter.drawText(this->rect(),m_curTime,option);
    }
}
