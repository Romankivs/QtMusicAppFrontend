#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include "backendcommunicator.h"

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(BackendCommunicator *backend, QWidget *parent = nullptr);

signals:
    void loginSuccess();

private slots:
    void onLoginButtonClicked();
    void onRegisterButtonClicked();
    void onLoginFinished(bool success);
    void onRegistrationFinished(bool success);

private:
    QLineEdit *usernameField;
    QLineEdit *passwordField;
    QPushButton *loginButton;
    QPushButton *registerButton;
    BackendCommunicator *backend;
};

#endif // LOGINWINDOW_H
