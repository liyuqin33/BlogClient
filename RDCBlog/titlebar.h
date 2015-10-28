#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <QToolButton>
#include <QPushButton>
#include <QIcon>
#include <QLabel>
#include <QHBoxLayout>
#include <QGridLayout>

#define TITLE_HEIGHT 25

class TitleBar : public QFrame
{
    Q_OBJECT
public:
    explicit TitleBar(QWidget *parent = 0);
    ~TitleBar();
    void setName(QString s);
    QString getName();
    bool getLockFlag();
private:
    QIcon _maxIcon;
    QIcon _normalIcon;
    QIcon _unlockIcon;
    QIcon _lockIcon;

    QLabel *_nameBtn;
    QToolButton *_logoButton;
    QToolButton *_settingButton;
    QToolButton *_skinButton;
    QToolButton *_lockButton;
    QToolButton *_minButton;
    QToolButton *_maxButton;
    QToolButton *_closeButton;

    QPushButton *_homeBtn;              //首页
    QPushButton *_blogBtn;              //博客
    QPushButton *_picBtn;               //照片墙
    QPushButton *_personBtn;            //个人中心
    QPushButton *_communitBtn;          //社区动态
    QPushButton *_aboutBtn;             //关于研发
    bool _isMax;                        //是否最大化正常化
    bool _isLock;                       //是否锁定

    void initUI();
    void initLayout();
    void initConnect();

signals:
    //每个按钮的信号
    void clickSkinBtn();
    void clickSettingBtn();
    void clickMinBtn();
    void clickUnlockBtn();
    void clickLockBtn();
    void clickMaxBtn();
    void clickNormalBtn();
    void clickCloseBtn();

    void clickFunctionBtn(int);         //各个功能widget对应一个int，与QStackWidget一一对应


public slots:
    void swithMaxNormalBtn();
    void swithLockBtn();

    void clickHomeBtn();
    void clickBlogBtn();
    void clickPicBtn();
    void clickPersonBtn();
    void clickCommunitBtn();
    void clickAboutBtn();
};

#endif // TITLEBAR_H
