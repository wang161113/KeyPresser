#ifndef KEYPRESSER_H
#define KEYPRESSER_H

#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QSettings>
#include <windows.h>
#include <vector>

class KeyPresser : public QWidget {
    Q_OBJECT

public:
    KeyPresser(QWidget *parent = nullptr);
    ~KeyPresser();

    static KeyPresser *instance;
    QLabel *instructionLabel;
public slots:
    void selectWindow();
    void startPressing();
    void stopPressing();
    void aboutMe();
    void pressSpace();
    void pressKeys(int index);

private:
    static void CALLBACK WinEventProc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);

    QLabel *selectedWindowLabel;
    QCheckBox *spaceCheckBox;
    QLineEdit *spaceIntervalLineEdit;
    QCheckBox *keyCheckBoxes[10];
    QComboBox *keyCombos[10];
    QLineEdit *intervalLineEdits[10];
    QTimer *spaceTimer;
    std::vector<QTimer*> timers;
    HWND targetHwnd = nullptr;


    void populateKeyCombos(QComboBox *comboBox);
    void stopAllTimers();
    void loadSettings();
    void saveSettings();
    void attachToTargetWindow();
    void detachFromTargetWindow();
};

#endif // KEYPRESSER_H
