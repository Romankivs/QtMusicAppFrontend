#include "loginwindow.h"
#include <QVBoxLayout>

LoginWindow::LoginWindow(BackendCommunicator *backend, QWidget *parent)
    : QWidget(parent), backend(backend)
{
    setWindowTitle("Login");

    QLabel *usernameLabel = new QLabel("Username:", this);
    usernameField = new QLineEdit(this);

    QLabel *passwordLabel = new QLabel("Password:", this);
    passwordField = new QLineEdit(this);
    passwordField->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("Login", this);
    registerButton = new QPushButton("Register", this);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(usernameLabel);
    layout->addWidget(usernameField);
    layout->addWidget(passwordLabel);
    layout->addWidget(passwordField);
    layout->addWidget(loginButton);
    layout->addWidget(registerButton);
    setLayout(layout);

    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginButtonClicked);
    connect(registerButton, &QPushButton::clicked, this, &LoginWindow::onRegisterButtonClicked);
    connect(backend, &BackendCommunicator::loginFinished, this, &LoginWindow::onLoginFinished);
    connect(backend, &BackendCommunicator::registrationFinished, this, &LoginWindow::onRegistrationFinished);
}

void LoginWindow::onLoginButtonClicked()
{
    QString username = usernameField->text();
    QString password = passwordField->text();
    backend->login(username, password);
}

void LoginWindow::onRegisterButtonClicked()
{
    QString username = usernameField->text();
    QString password = passwordField->text();
    backend->registerUser(username, password);
}

void LoginWindow::onLoginFinished(bool success)
{
    if (success) {
        emit loginSuccess();
        close();
    } else {
        QMessageBox::warning(this, "Login Failed", "Invalid username or password.");
    }
}

void LoginWindow::onRegistrationFinished(bool success)
{
    if (success) {
        QMessageBox::information(this, "Registration Success", "You have successfully registered.");
    } else {
        QMessageBox::warning(this, "Registration Failed", "Registration failed. Try a different username.");
    }
}
