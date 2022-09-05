#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "fellowlistwidget.h"
#include "searchfellowdlg.h"
#include "recvtextedit.h"
#include "feiqlib/feiqengine.h"
#include "filemanagerdlg.h"
#include "settings.h"
#include <unordered_map>
#include "chooseemojidlg.h"
#include <QFileInfo>
#include "sendtextedit.h"

using namespace std;

namespace Ui {
class MainWindow;
}

class FeiqWin;

struct UnshownMessage
{
    shared_ptr<ViewEvent> event;
    bool replied = false;
    bool read = false;
    long notifyId = 0;

    bool isUnread()
    {
        return !replied && !read;
    }
};

Q_DECLARE_METATYPE(shared_ptr<ViewEvent>)
class MainWindow : public QMainWindow, IFeiqView
{
    Q_OBJECT

    friend class FeiqWin;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setFeiqWin(FeiqWin* feiqWin);

public slots:
    void onNotifyClicked(const QString& fellowIp);
    void onNotifyReplied(long notifyId, const QString& fellowIp, const QString& reply);

protected:
    void enterEvent(QEvent *event);

signals:
    void showErrorAndQuit(const QString& text);
    void statChanged(FileTask* fileTask);
    void progressChanged(FileTask* fileTask);
    void feiqViewEvent(shared_ptr<ViewEvent> event);

private slots:
    void finishSearch(const Fellow* fellow);
    void openSettings();
    void openSearchDlg();
    void openDownloadDlg();
    void onShowErrorAndQuit(const QString& text);
    void navigateToFileTask(IdType packetNo, IdType fileId, bool upload);
    void sendKnock();
    void sendText();
    void openChartTo(const Fellow* fellow);
    void handleFeiqViewEvent(shared_ptr<ViewEvent> event);
    void refreshFellowList();
    void addFellow();
    void openChooseEmojiDlg();
    void sendFile();
    void sendFile(string filepath);
    void sendFiles(QList<QFileInfo> files);

private:
    void userAddFellow(QString ip);
    long showNotification(const Fellow* fellow, const QString& text);
    shared_ptr<Fellow> checkCurFellow();
    void showResult(pair<bool, string> ret, const Content *content);
    vector<const Fellow*> fellowSearchDriver(const QString& text);
    void initFeiq();
    void readEvent(const ViewEvent* event);
    void setBadgeNumber(int number);
    QString simpleTextOf(const Content* content);

    UnshownMessage& addUnshownMessage(const Fellow *fellow, shared_ptr<ViewEvent> event);
    UnshownMessage *findUnshownMessage(int id);
    void notifyUnshown(UnshownMessage &umsg);
    void updateUnshownHint(const Fellow* fellow);
    int getUnreadCount();
    void flushUnshown(const Fellow* fellow);

    // IFileTaskObserver interface
public:
    void onStateChanged(FileTask *fileTask);
    void onProgress(FileTask *fileTask);

    // IFeiqView interface
public:
    void onEvent(shared_ptr<ViewEvent> event);

private:
    Ui::MainWindow *ui;
    FellowListWidget mFellowList;
    SearchFellowDlg* mSearchFellowDlg;
    FileManagerDlg* mDownloadFileDlg;
    ChooseEmojiDlg* mChooseEmojiDlg;
    Settings* mSettings;
    FeiqEngine mFeiq;
    RecvTextEdit* mRecvTextEdit;
    SendTextEdit* mSendTextEdit;
    QString mTitle;
    unordered_map<const Fellow*, list<UnshownMessage>> mUnshownEvents;
    FeiqWin* mFeiqWin = nullptr;
};

#endif // MAINWINDOW_H
