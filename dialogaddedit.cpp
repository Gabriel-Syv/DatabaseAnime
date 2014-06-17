#include "dialogaddedit.h"
#include "ui_dialogaddedit.h"

#include <QSettings>
#include <QFileDialog>
#include <QtSql>

#include <QMessageBox>
#include <QErrorMessage>
#include <QDebug>

DialogAddEdit::DialogAddEdit(bool isEditRole, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddEdit)
{
    ui->setupUi(this);
    QSettings settings;
    if( settings.value( "enableElem/FieldsForEdit/OrigTitle", true ).toBool() ){
        this->LineEdit_OrigTitle = new QLineEdit(this);
        this->LineEdit_OrigTitle->setObjectName("LineEdit_OrigTitle");
        this->LineEdit_OrigTitle->setPlaceholderText( tr("Оригинальное название", "Placeholder text, dialogAddEdit") );
        ui->VLay_OrigTitle->addWidget( this->LineEdit_OrigTitle );
    }
    if( settings.value( "enableElem/FieldsForEdit/Director", true ).toBool() ){
        this->LineEdit_Director = new QLineEdit(this);
        this->LineEdit_Director->setObjectName("LineEdit_Director");
        this->LineEdit_Director->setPlaceholderText( tr("Режиссёр", "Placeholder text, dialogAddEdit") );
        ui->HLay_DirectorAndSound->addWidget( this->LineEdit_Director );
    }
    if( settings.value( "enableElem/FieldsForEdit/PostScoring", true ).toBool() ){
        this->LineEdit_PostScoring = new QLineEdit(this);
        this->LineEdit_PostScoring->setObjectName("LineEdit_PostScoring");
        this->LineEdit_PostScoring->setPlaceholderText( tr("Озвучка", "Placeholder text, dialogAddEdit") );
        ui->HLay_DirectorAndSound->addWidget( this->LineEdit_PostScoring );
    }
    TableModel_Tags = new QSqlTableModel;
    TableModel_Tags->setTable("AnimeTags");
    TableModel_Tags->select();

    ui->ListView_Tags->setModel( TableModel_Tags );
    ui->ListView_Tags->setWrapping( true );
    ui->ListView_Tags->setModelColumn(1);
    ui->ListView_Tags->setSelectionMode( QAbstractItemView::MultiSelection );

    if( isEditRole ){
        ;// Set values
    }
}

DialogAddEdit::~DialogAddEdit()
{
    delete ui;
}

void DialogAddEdit::on_BtnBox_reset()
{
    ui->CheckBox_LookLater->setChecked( false );
    ui->CheckBox_Editing->setChecked( false );

    ui->LineEdit_Title->clear();
    // optional
    QSettings settings;
    if( settings.value( "enableElem/FieldsForEdit/OrigTitle", true ).toBool() ){
        this->LineEdit_OrigTitle->clear();
    }
    if( settings.value( "enableElem/FieldsForEdit/Director", true ).toBool() ){
        this->LineEdit_Director->clear();
    }
    if( settings.value( "enableElem/FieldsForEdit/PostScoring", true ).toBool() ){
        this->LineEdit_PostScoring->clear();
    }

    ui->DateTimeEdit_Year->setDate( QDate(2000,1,1) );
    ui->SpinBox_Season->setValue(0);
    ui->ComboBox_Studio->clear();

    ui->SpinBox_aTV->setValue(0);
    ui->SpinBox_aOVA->setValue(0);
    ui->SpinBox_aONA->setValue(0);
    ui->SpinBox_aSpec->setValue(0);
    ui->SpinBox_aFilm->setValue(0);

    ui->SpinBox_vTV->setValue(0);
    ui->SpinBox_vOVA->setValue(0);
    ui->SpinBox_vONA->setValue(0);
    ui->SpinBox_vSpec->setValue(0);
    ui->SpinBox_vFilm->setValue(0);

    ui->LineEdit_Tags->clear();
    ui->ListView_Tags->clearSelection();
    ui->PlainTextEdit_Description->clear();
    ui->LineEdit_Dir->clear();
    ui->LineEdit_URL->clear();
}

void DialogAddEdit::on_BtnBox_clicked(QAbstractButton *button)
{
    switch( ui->BtnBox->buttonRole( button ) ){
        case 7:
            emit on_BtnBox_reset();
            break;
        default:
            this->close();
    }
}

bool DialogAddEdit::insert_AnimeSerials(){
    QSqlQuery query;
    query.prepare("INSERT INTO animeSerials("
                  "isHaveLooked, isEditingDone, Title,"
                  "OrigTitle, Director, PostScoring,"
                  "SeriesTV, SeriesOVA, SeriesONA, SeriesSpecial, SeriesFilm,"
                  "vSeriesTV, vSeriesOVA, vSeriesONA, vSeriesSpecial, vSeriesFilm,"
                  "Year, Season, Studios,"
                  "Tags, Description,"
                  "URL, Dir, ImagePath"
                  ") VALUES "
                  "(:isHaveLooked, :isEditingDone, :Title,"
                  ":OrigTitle, :Director, :PostScoring,"
                  ":SeriesTV, :SeriesOVA, :SeriesONA, :SeriesSpecial, :SeriesFilm,"
                  ":vSeriesTV, :vSeriesOVA, :vSeriesONA, :vSeriesSpecial, :vSeriesFilm,"
                  ":Year, :Season, :Studios,"
                  ":Tags, :Description,"
                  ":URL, :Dir, :ImagePath)"
                  );

    query.bindValue(":isHaveLooked",  !ui->CheckBox_LookLater->isChecked() );
    query.bindValue(":isEditingDone", !ui->CheckBox_Editing->isChecked() );
    query.bindValue(":Title",         ui->LineEdit_Title->text() );

    QSettings settings;
    if( settings.value( "enableElem/FieldsForEdit/OrigTitle", true ).toBool() ){
        query.bindValue(":OrigTitle", this->LineEdit_OrigTitle->text() );
    }
    if( settings.value( "enableElem/FieldsForEdit/Director", true ).toBool() ){
        query.bindValue(":Director", this->LineEdit_Director->text() );
    }
    if( settings.value( "enableElem/FieldsForEdit/PostScoring", true ).toBool() ){
        query.bindValue(":PostScoring", this->LineEdit_PostScoring->text() );
    }

    query.bindValue(":SeriesTV",      ui->SpinBox_aTV->value()   );
    query.bindValue(":SeriesOVA",     ui->SpinBox_aOVA->value()  );
    query.bindValue(":SeriesONA",     ui->SpinBox_aONA->value()  );
    query.bindValue(":SeriesSpecial", ui->SpinBox_aSpec->value() );
    query.bindValue(":SeriesFilm",    ui->SpinBox_aFilm->value() );
    query.bindValue(":vSeriesTV",     ui->SpinBox_vTV->value()   );
    query.bindValue(":vSeriesOVA",    ui->SpinBox_vOVA->value()  );
    query.bindValue(":vSeriesONA",    ui->SpinBox_vONA->value()  );
    query.bindValue(":vSeriesSpecial",ui->SpinBox_vSpec->value() );
    query.bindValue(":vSeriesFilm",   ui->SpinBox_vFilm->value() );
    query.bindValue(":Year",          ui->DateTimeEdit_Year->date() );
    query.bindValue(":Season",        ui->SpinBox_Season->value()   );
    query.bindValue(":Studios",       ui->ComboBox_Studio->currentIndex() );
    query.bindValue(":Tags",          ui->LineEdit_Tags->text() ); // #Bug
    query.bindValue(":Description",   ui->PlainTextEdit_Description->toPlainText() );
    query.bindValue(":URL",           ui->LineEdit_URL->text() );
    query.bindValue(":Dir",           ui->LineEdit_Dir->text() );
    query.bindValue(":ImagePath",     ui->Lbl_ImageCover->getImagePath() ); // #Bug

    if( !query.exec() ){
        qDebug() << "Cannot insert data in table animeSerials: " << query.lastError();
        (new QErrorMessage(0))->showMessage( query.lastError().text() );
        return false;
    }
    return true;
}

void DialogAddEdit::on_BtnBox_accepted()
{
//    QMessageBox::information(this,"Accept","Типа добавлено.");
    insert_AnimeSerials();
    this->close();
}

void DialogAddEdit::on_BtnBox_rejected()
{
    this->close();
}

void DialogAddEdit::on_SpinBox_aTV_valueChanged(int arg1)
{
    ui->SpinBox_vTV->setMaximum(arg1);
}

void DialogAddEdit::on_SpinBox_aOVA_valueChanged(int arg1)
{
    ui->SpinBox_vOVA->setMaximum(arg1);
}

void DialogAddEdit::on_SpinBox_aONA_valueChanged(int arg1)
{
    ui->SpinBox_vONA->setMaximum(arg1);
}

void DialogAddEdit::on_SpinBox_aSpec_valueChanged(int arg1)
{
    ui->SpinBox_vSpec->setMaximum(arg1);
}

void DialogAddEdit::on_SpinBox_aFilm_valueChanged(int arg1)
{
    ui->SpinBox_vFilm->setMaximum(arg1);
}

void DialogAddEdit::on_toolButton_clicked()
{
    ui->LineEdit_Dir->setText( QFileDialog::getExistingDirectory(this, tr("Выберите директорию с видео-файлами"), ui->LineEdit_Dir->text() ) );
}
