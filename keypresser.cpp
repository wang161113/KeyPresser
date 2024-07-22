#include "keypresser.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QGridLayout>
#include <QMessageBox>
#include <QIntValidator>
#include <QKeyEvent>
#include <QApplication>
#include "AboutMeDlg.h"

// 全局钩子处理函数
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        if (wParam == WM_KEYDOWN) {
            KBDLLHOOKSTRUCT* kbStruct = (KBDLLHOOKSTRUCT*)lParam;
            if (kbStruct->vkCode == VK_HOME && (KeyPresser::instance)) {
                // 处理Home键事件
                bool bRuning = (KeyPresser::instance)->instructionLabel->text() == "运行中";
                bRuning ? (KeyPresser::instance)->stopPressing() : (KeyPresser::instance)->startPressing();
            }
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}
HHOOK hook;
// 安装钩子
void InstallHook() {
    hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandle(NULL), 0);
    // 可以保存hook以便卸载
}

// 卸载钩子
void UninstallHook() {
    UnhookWindowsHookEx(hook);
}

KeyPresser *KeyPresser::instance = nullptr;

KeyPresser::KeyPresser(QWidget *parent) : QWidget(parent) {
    instance = this;

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *label = new QLabel("选择窗口:", this);
    layout->addWidget(label);

    selectedWindowLabel = new QLabel("未选择窗口", this);
    selectedWindowLabel->setStyleSheet("color: green;");
    layout->addWidget(selectedWindowLabel);

    QPushButton *selectButton = new QPushButton("选择窗口", this);
    layout->addWidget(selectButton);

    QHBoxLayout *spaceLayout = new QHBoxLayout();
    spaceCheckBox = new QCheckBox(this);
    spaceLayout->addWidget(spaceCheckBox);

    QLabel *spaceLabel = new QLabel("空格键时间间隔 (毫秒):", this);
    spaceLayout->addWidget(spaceLabel);

    spaceIntervalLineEdit = new QLineEdit(this);
    spaceIntervalLineEdit->setValidator(new QIntValidator(this));
    spaceIntervalLineEdit->setText("1000");
    spaceLayout->addWidget(spaceIntervalLineEdit);

    layout->addLayout(spaceLayout);

    QLabel *keysLabel = new QLabel("自定义按键和时间间隔 (毫秒):", this);
    layout->addWidget(keysLabel);

    QGridLayout *keysLayout = new QGridLayout();
    for (int i = 0; i < 10; ++i) {
        keyCheckBoxes[i] = new QCheckBox(this);
        keysLayout->addWidget(keyCheckBoxes[i], i, 0);

        keyCombos[i] = new QComboBox(this);
        populateKeyCombos(keyCombos[i]);
        keysLayout->addWidget(keyCombos[i], i, 1);

        intervalLineEdits[i] = new QLineEdit(this);
        intervalLineEdits[i]->setValidator(new QIntValidator(this));
        intervalLineEdits[i]->setText("1000");
        keysLayout->addWidget(intervalLineEdits[i], i, 2);

        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, [this, i]() { pressKeys(i); });
        timers.push_back(timer);
    }
    layout->addLayout(keysLayout);

    QPushButton *startButton = new QPushButton("开始", this);
    layout->addWidget(startButton);

    QPushButton *stopButton = new QPushButton("停止", this);
    layout->addWidget(stopButton);

    QLabel *labelPrompt = new QLabel("修改配置后需点击开始按钮以使更改生效。", this);
    labelPrompt->setStyleSheet("color: red;");
    layout->addWidget(labelPrompt);

    instructionLabel = new QLabel("停止中", this);
    instructionLabel->setStyleSheet("color: green;");
    layout->addWidget(instructionLabel);

    QPushButton *aboutButton = new QPushButton("关于", this);
    layout->addWidget(aboutButton);


    connect(selectButton, &QPushButton::clicked, this, &KeyPresser::selectWindow);
    connect(startButton, &QPushButton::clicked, this, &KeyPresser::startPressing);
    connect(stopButton, &QPushButton::clicked, this, &KeyPresser::stopPressing);
    connect(aboutButton, &QPushButton::clicked, this, &KeyPresser::aboutMe);

    spaceTimer = new QTimer(this);
    connect(spaceTimer, &QTimer::timeout, this, &KeyPresser::pressSpace);

    loadSettings();
    InstallHook();
}

KeyPresser::~KeyPresser() {
    saveSettings();
    UninstallHook();
}

void KeyPresser::populateKeyCombos(QComboBox *comboBox) {
    comboBox->addItem("F1", VK_F1);
    comboBox->addItem("F2", VK_F2);
    comboBox->addItem("F3", VK_F3);
    comboBox->addItem("F4", VK_F4);
    comboBox->addItem("F5", VK_F5);
    comboBox->addItem("F6", VK_F6);
    comboBox->addItem("F7", VK_F7);
    comboBox->addItem("F8", VK_F8);
    comboBox->addItem("F9", VK_F9);
    comboBox->addItem("F10", VK_F10);
    comboBox->addItem("F11", VK_F11);
    comboBox->addItem("F12", VK_F12);
    comboBox->addItem("A", 'A');
    comboBox->addItem("B", 'B');
    comboBox->addItem("C", 'C');
    comboBox->addItem("D", 'D');
    comboBox->addItem("E", 'E');
    comboBox->addItem("F", 'F');
    comboBox->addItem("G", 'G');
    comboBox->addItem("H", 'H');
    comboBox->addItem("I", 'I');
    comboBox->addItem("J", 'J');
    comboBox->addItem("K", 'K');
    comboBox->addItem("L", 'L');
    comboBox->addItem("M", 'M');
    comboBox->addItem("N", 'N');
    comboBox->addItem("O", 'O');
    comboBox->addItem("P", 'P');
    comboBox->addItem("Q", 'Q');
    comboBox->addItem("R", 'R');
    comboBox->addItem("S", 'S');
    comboBox->addItem("T", 'T');
    comboBox->addItem("U", 'U');
    comboBox->addItem("V", 'V');
    comboBox->addItem("W", 'W');
    comboBox->addItem("X", 'X');
    comboBox->addItem("Y", 'Y');
    comboBox->addItem("Z", 'Z');
    comboBox->addItem("0", '0');
    comboBox->addItem("1", '1');
    comboBox->addItem("2", '2');
    comboBox->addItem("3", '3');
    comboBox->addItem("4", '4');
    comboBox->addItem("5", '5');
    comboBox->addItem("6", '6');
    comboBox->addItem("7", '7');
    comboBox->addItem("8", '8');
    comboBox->addItem("9", '9');
    comboBox->addItem("Space", VK_SPACE);
    comboBox->addItem("Enter", VK_RETURN);
    comboBox->addItem("Tab", VK_TAB);
    comboBox->addItem("Esc", VK_ESCAPE);
    comboBox->addItem("Backspace", VK_BACK);
    comboBox->addItem("Insert", VK_INSERT);
    comboBox->addItem("Delete", VK_DELETE);
    comboBox->addItem("Home", VK_HOME);
    comboBox->addItem("End", VK_END);
    comboBox->addItem("Page Up", VK_PRIOR);
    comboBox->addItem("Page Down", VK_NEXT);
    comboBox->addItem("Left Arrow", VK_LEFT);
    comboBox->addItem("Right Arrow", VK_RIGHT);
    comboBox->addItem("Up Arrow", VK_UP);
    comboBox->addItem("Down Arrow", VK_DOWN);
}

void KeyPresser::selectWindow() {
    SetWinEventHook(EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND, NULL, WinEventProc, 0, 0, WINEVENT_OUTOFCONTEXT);
    selectedWindowLabel->setText("请点击目标窗口...");
}

void KeyPresser::startPressing() {
    if(!targetHwnd)
    {
        QMessageBox::warning(this,"警告","请选择窗口后，再点击开始！");
        return;
    }
    instructionLabel->setText("运行中");
    stopAllTimers();
    attachToTargetWindow();
    if (targetHwnd) {
        if (spaceCheckBox->isChecked()) {
            pressSpace();
            spaceTimer->start(spaceIntervalLineEdit->text().toInt());
        }
        for (int i = 0; i < 10; ++i) {
            if (keyCheckBoxes[i]->isChecked() && keyCombos[i]->currentIndex() != -1) {
                pressKeys(i);
                timers[i]->start(intervalLineEdits[i]->text().toInt());
            }
        }
    }
}

void KeyPresser::stopPressing() {
    instructionLabel->setText("停止中");
    stopAllTimers();
    detachFromTargetWindow();
}

void KeyPresser::aboutMe()
{
    AboutMeDlg dlg(this);
    dlg.exec();
}

void KeyPresser::stopAllTimers() {
    spaceTimer->stop();
    for (QTimer *timer : timers) {
        timer->stop();
    }
}

void KeyPresser::pressSpace() {
    if (targetHwnd) {
        DWORD dwVKFkeyData; // lParam 参数值
        WORD dwScanCode = MapVirtualKey(VK_SPACE, MAPVK_VK_TO_VSC); // 获取虚拟按键的键盘扫描码

        // 模拟按下空格键
        dwVKFkeyData = 1;
        dwVKFkeyData |= dwScanCode << 16;
        dwVKFkeyData |= 0 << 24;
        dwVKFkeyData |= 1 << 29;
        PostMessage(targetHwnd, WM_KEYDOWN, VK_SPACE, dwVKFkeyData);

        // 重置 dwVKFkeyData 以确保 WM_KEYUP 消息正确
        dwVKFkeyData = 1;
        dwVKFkeyData |= dwScanCode << 16;
        dwVKFkeyData |= 0 << 24;
        dwVKFkeyData |= 1 << 29;
        dwVKFkeyData |= 3 << 30;
        PostMessage(targetHwnd, WM_KEYUP, VK_SPACE, dwVKFkeyData);

    }
}

void KeyPresser::pressKeys(int index) {
    if (targetHwnd) {
        int key = keyCombos[index]->currentData().toInt();

        DWORD dwVKFkeyData; // lParam 参数值
        WORD dwScanCode = MapVirtualKey(key, MAPVK_VK_TO_VSC); // 获取虚拟按键的键盘扫描码

        dwVKFkeyData = 1;
        dwVKFkeyData |= dwScanCode << 16;
        dwVKFkeyData |= 0 << 24;
        dwVKFkeyData |= 1 << 29;
        PostMessage(targetHwnd, WM_KEYDOWN, key, dwVKFkeyData);

        // 重置 dwVKFkeyData 以确保 WM_KEYUP 消息正确
        dwVKFkeyData = 1;
        dwVKFkeyData |= dwScanCode << 16;
        dwVKFkeyData |= 0 << 24;
        dwVKFkeyData |= 1 << 29;
        dwVKFkeyData |= 3 << 30;
        PostMessage(targetHwnd, WM_KEYUP, key, dwVKFkeyData);
    }
}

void KeyPresser::loadSettings() {
    QSettings settings("FinnSoft", "KeyPresser");

    spaceCheckBox->setChecked(settings.value("spaceCheckBox", false).toBool());
    spaceIntervalLineEdit->setText(settings.value("spaceIntervalLineEdit", "1000").toString());

    for (int i = 0; i < 10; ++i) {
        keyCheckBoxes[i]->setChecked(settings.value(QString("keyCheckBox%1").arg(i), false).toBool());
        keyCombos[i]->setCurrentIndex(settings.value(QString("keyCombo%1").arg(i), 0).toInt());
        intervalLineEdits[i]->setText(settings.value(QString("intervalLineEdit%1").arg(i), "1000").toString());
    }
}

void KeyPresser::saveSettings() {
    QSettings settings("FinnSoft", "KeyPresser");

    settings.setValue("spaceCheckBox", spaceCheckBox->isChecked());
    settings.setValue("spaceIntervalLineEdit", spaceIntervalLineEdit->text());

    for (int i = 0; i < 10; ++i) {
        settings.setValue(QString("keyCheckBox%1").arg(i), keyCheckBoxes[i]->isChecked());
        settings.setValue(QString("keyCombo%1").arg(i), keyCombos[i]->currentIndex());
        settings.setValue(QString("intervalLineEdit%1").arg(i), intervalLineEdits[i]->text());
    }
}

void KeyPresser::attachToTargetWindow() {
    if (targetHwnd) {
        DWORD targetThreadId = GetWindowThreadProcessId(targetHwnd, NULL);
        DWORD currentThreadId = GetCurrentThreadId();
        AttachThreadInput(currentThreadId, targetThreadId, TRUE);  // 关联当前线程与目标窗口的线程
    }
}

void KeyPresser::detachFromTargetWindow() {
    if (targetHwnd) {
        DWORD targetThreadId = GetWindowThreadProcessId(targetHwnd, NULL);
        DWORD currentThreadId = GetCurrentThreadId();
        AttachThreadInput(currentThreadId, targetThreadId, FALSE);  // 解除当前线程与目标窗口的线程关联
    }
}

void CALLBACK KeyPresser::WinEventProc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime) {
    if (event == EVENT_SYSTEM_FOREGROUND) {
        wchar_t windowTitle[256];
        GetWindowText(hwnd, windowTitle, 256);
        if (KeyPresser::instance) {
            KeyPresser::instance->targetHwnd = hwnd;
            KeyPresser::instance->selectedWindowLabel->setText(QString::fromWCharArray(windowTitle));
            UnhookWinEvent(hWinEventHook);
        }
    }
}
