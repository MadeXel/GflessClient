#include "captchadialog.h"
#include "ui_captchadialog.h"

CaptchaDialog::CaptchaDialog(const QString &gfChallengeId, SyncNetworAccesskManager *netManager, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CaptchaDialog)
{
    ui->setupUi(this);
    captcha = new CaptchaSolver(gfChallengeId, QLocale().name().replace("_", "-"), netManager, this);

    auto captchaInfo = captcha->getCaptchaInfo();

    QString type = captchaInfo.value("type").toString();
    QString script = captchaInfo.value("script").toString();

    if (type == "gf-image-drop-captcha") {
        initImages();
    }
    else if (type == "gf-pow-captcha") {
        QMessageBox::warning(
            this,
            "Unsupported type of captcha",
            "The type of captcha you received is not supported\nYou need to resolve it using the gameforge client or your browser."
        );
    }
    else {
        QMessageBox::critical(
            this,
            "Unsupported type of captcha",
            "Unsupported type of captcha\n\nType: " + type + "\nScript: " + script
                + "\n\nYou need to resolve it using the gameforge client or your browser."
        );
    }
}

CaptchaDialog::~CaptchaDialog()
{
    delete captcha;
    delete ui;
}

void CaptchaDialog::initImages()
{
    captcha->getChallenge();
    ui->textLabel->setPixmap(QPixmap::fromImage(captcha->getTextImage()));
    ui->dragIconsLabel->setPixmap(QPixmap::fromImage(captcha->getDragIcons()));
    ui->dropTargetLabel->setPixmap(QPixmap::fromImage(captcha->getDropTargetImage()));
}

void CaptchaDialog::on_sendAnswerBton_clicked()
{
    int answer = ui->answerSpinbox->value() - 1;

    if (captcha->sendAnswer(answer))
    {
        QMessageBox::information(this, "Success", "Captcha solved successfully");
        accept();
    }
    else
    {
        QMessageBox::critical(this, "Error", "Incorrect answer");
        initImages();
    }
}

