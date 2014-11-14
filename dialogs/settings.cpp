#include "settings.h"
#include "ui_settings.h"

Settings::Settings(MngrConnection &MngrCon, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings), MngrConnect(MngrCon)
{
    ui->setupUi(this);
    QSettings settings;
    settings.beginGroup("DialogSettings");
        this->restoreGeometry( settings.value("Geometry").toByteArray() );
        settings.beginGroup("Spletter");
            ui->splitter->restoreGeometry( settings.value("Geometry").toByteArray() );
            ui->splitter->restoreState( settings.value("State").toByteArray() );
        settings.endGroup(/*Spletter*/);
    settings.endGroup(/*DialogSettings*/);
    ui->LineEdit_Export_FilePath->setText( DefinesPath::home() );
    ui->LineEdit_Import_FilePath->setText( DefinesPath::home() );

    settings.beginGroup("ActiveSections");
        bool b1 = settings.value( "Anime",   true ).toBool();
        bool b2 = settings.value( "Manga",  false ).toBool();
        bool b3 = settings.value( "AMV",    false ).toBool();
        bool b4 = settings.value( "Dorama", false ).toBool();
    settings.endGroup();

    settings.beginGroup("OptionalFields");
        settings.beginGroup("Anime");
            bool a1 = settings.value( "AltTitle",    false ).toBool();
            bool a2 = settings.value( "Director",    false ).toBool();
            bool a3 = settings.value( "Postscoring", false ).toBool();
        settings.endGroup();

        settings.beginGroup("Manga");
            bool m1 = settings.value( "AltTitle",    false ).toBool();
            bool m2 = settings.value( "Author",      false ).toBool();
            bool m3 = settings.value( "Translation", false ).toBool();
        settings.endGroup();

        settings.beginGroup("Dorama");
            bool d1 = settings.value( "AltTitle", false ).toBool();
            bool d2 = settings.value( "Director", false ).toBool();
        settings.endGroup();
    settings.endGroup();

    settings.beginGroup("Network");
        bool checkUpdates = settings.value("CheckUpdates", true).toBool();
        bool searchOnShikimori = settings.value("SearchOnShikimori", true).toBool();
    settings.endGroup(/*Network*/);

    bool SwitchCoverOrDir = settings.value( "SwitchCoverOrDir", true ).toBool();

    ui->ChBox_AS_Anime->setChecked( b1 );
    ui->ChBox_AS_Manga->setChecked( b2 );
    ui->ChBox_AS_Amv->setChecked( b3 );
    ui->ChBox_AS_Dorama->setChecked( b4 );

    ui->ChBox_OptField_Anime_AltTitle->setChecked( a1 );
    ui->ChBox_OptField_Anime_Director->setChecked( a2 );
    ui->ChBox_OptField_Anime_Postscoring->setChecked( a3 );

    ui->ChBox_OptField_Manga_AltTitle->setChecked( m1 );
    ui->ChBox_OptField_Manga_Author->setChecked( m2 );
    ui->ChBox_OptField_Manga_Translation->setChecked( m3 );

    ui->ChBox_OptField_Dorama_AltTitle->setChecked( d1 );
    ui->ChBox_OptField_Dorama_Director->setChecked( d2 );

    ui->ChBox_CheckForUpdate->setChecked( checkUpdates );
    ui->ChBox_SwitchCoverOrDir->setChecked( SwitchCoverOrDir );
    ui->ChBox_SearchOnShikimori->setChecked( searchOnShikimori );

    QLocale::Language set_language = static_cast<QLocale::Language>(settings.value( "Language", QLocale::English ).toInt());

    ui->ComboBox_Language->addItem( tr("<System>"), 0 );

    QMap<QLocale::Language,QString> langList = DbaLocalization::readExistsLocalizations( DefinesPath::share() );
    int i(0);
    foreach (QString langName, langList) {
        ++i;
        ui->ComboBox_Language->addItem(langName, langList.key(langName));
        if( set_language == langList.key(langName) )
            ui->ComboBox_Language->setCurrentIndex(i);
    }
    // Sorting
    {  // Sorting
        ui->ComboBox_ItemList_Sorting->addItem(tr("None"), Sort::none);
        ui->ComboBox_ItemList_Sorting->addItem(tr("ASC"),  Sort::asc);
        ui->ComboBox_ItemList_Sorting->addItem(tr("DESC"), Sort::desc);
        ui->ComboBox_ItemList_Sorting->addItem(tr("Year"), Sort::year);

        Sort::sort sort = static_cast<Sort::sort>( settings.value( "Sorting", Sort::asc ).toInt() );
        ui->ComboBox_ItemList_Sorting->setCurrentIndex( sort );
    }
    // Work dir
    ui->LineEdit_WorkDir->setText( QDir::toNativeSeparators( DefinesPath::appData() ) );

    // Displayed field //DisplayedField
    {   // Displayed field //DisplayedField
        Tables::UniformField::field displayedField = static_cast<Tables::UniformField::field>( settings.value( "DisplayedField", Tables::UniformField::Title ).toInt() );
        ui->ComboBox_ItemList_DisplayedField->addItem(tr("Title"), Tables::UniformField::Title);
        ui->ComboBox_ItemList_DisplayedField->addItem(tr("Alternative title"), Tables::UniformField::AltTitle);
        if( displayedField == Tables::UniformField::Title )
            ui->ComboBox_ItemList_DisplayedField->setCurrentIndex( 0 );
        else
            ui->ComboBox_ItemList_DisplayedField->setCurrentIndex( 1 );
    }
}

Settings::~Settings()
{
    QSettings settings;
    settings.beginGroup("DialogSettings");
        settings.setValue("Geometry", this->saveGeometry() );

        settings.beginGroup("Spletter");
            settings.setValue("Geometry", ui->splitter->saveGeometry() );
            settings.setValue("State", ui->splitter->saveState() );
        settings.endGroup(/*Spletter*/);
    settings.endGroup(/*Dialog*/);
    delete ui;
}

Sort::sort Settings::getSort()
{
    return static_cast<Sort::sort>( ui->ComboBox_ItemList_Sorting->currentIndex() );
}

Tables::UniformField::field Settings::getDisplaydField()
{
    return static_cast<Tables::UniformField::field>(ui->ComboBox_ItemList_DisplayedField->currentData().toInt());
}

bool Settings::getSwitchToDir()
{
    return ui->ChBox_SwitchCoverOrDir->isChecked();
}

bool Settings::getRestoreDefault()
{
    return restoreDefault;
}

QLocale::Language Settings::getLanguage()
{
    return static_cast<QLocale::Language>( ui->ComboBox_Language->currentData().toInt() );
}

void Settings::on_listWidget_currentRowChanged(int currentRow)
{
    ui->StackedWidget->setCurrentIndex(currentRow);
}

void Settings::on_BtnBox_clicked(QAbstractButton *button)
{
    switch( ui->BtnBox->buttonRole( button ) ){
        case  QDialogButtonBox::ApplyRole:
            on_BtnBox_accepted();
            this->close();
            break;
        case QDialogButtonBox::ResetRole:
            BtnBox_resetDefaults();
            break;
        default:
            this->close();
    }
}

void Settings::on_BtnBox_accepted()
{
    QSettings settings;

    settings.beginGroup("ActiveSections");
        settings.setValue( "Anime",  ui->ChBox_AS_Anime->isChecked() );
        settings.setValue( "Manga",  ui->ChBox_AS_Manga->isChecked() );
        settings.setValue( "AMV",    ui->ChBox_AS_Amv->isChecked() );
        settings.setValue( "Dorama", ui->ChBox_AS_Dorama->isChecked() );
    settings.endGroup();

    settings.beginGroup("OptionalFields");
        settings.beginGroup("Anime");
            settings.setValue( "AltTitle",   ui->ChBox_OptField_Anime_AltTitle->isChecked() );
            settings.setValue( "Director",    ui->ChBox_OptField_Anime_Director->isChecked() );
            settings.setValue( "Postscoring", ui->ChBox_OptField_Anime_Postscoring->isChecked() );
        settings.endGroup();

        settings.beginGroup("Manga");
            settings.setValue( "AltTitle",    ui->ChBox_OptField_Manga_AltTitle->isChecked() );
            settings.setValue( "Author",      ui->ChBox_OptField_Manga_Author->isChecked() );
            settings.setValue( "Translation", ui->ChBox_OptField_Manga_Translation->isChecked() );
        settings.endGroup();

        settings.beginGroup("Dorama");
            settings.setValue( "AltTitle",   ui->ChBox_OptField_Dorama_AltTitle->isChecked() );
            settings.setValue( "Director",   ui->ChBox_OptField_Dorama_Director->isChecked() );
        settings.endGroup();
    settings.endGroup();

    settings.setValue( "Language", ui->ComboBox_Language->currentData() );
    settings.setValue( "Sorting", ui->ComboBox_ItemList_Sorting->currentIndex() );

    settings.beginGroup("Network");
        settings.setValue( "CheckUpdates", ui->ChBox_CheckForUpdate->isChecked() );
        settings.setValue( "SearchOnShikimori", ui->ChBox_SearchOnShikimori->isChecked() );
    settings.endGroup(/*Network*/);

    settings.setValue( "SwitchCoverOrDir", ui->ChBox_SwitchCoverOrDir->isChecked() );

    if( QDir::isAbsolutePath( ui->LineEdit_WorkDir->text() ) )
        settings.setValue( "WorkDirectory", QDir(ui->LineEdit_WorkDir->text()).path() );
    else
        settings.remove("WorkDirectory");

    // Displayed field
    Tables::UniformField::field displayedField = static_cast<Tables::UniformField::field>( ui->ComboBox_ItemList_DisplayedField->currentData().toInt() );
    settings.setValue( "DisplayedField", displayedField );
}

void Settings::BtnBox_resetDefaults()
{
    restoreDefault = true;
    ui->ChBox_AS_Anime->setChecked( true );
    ui->ChBox_AS_Manga->setChecked( false );
    ui->ChBox_AS_Amv->setChecked( false );
    ui->ChBox_AS_Dorama->setChecked( false );

    ui->ChBox_OptField_Anime_AltTitle->setChecked( false );
    ui->ChBox_OptField_Anime_Director->setChecked( false );
    ui->ChBox_OptField_Anime_Postscoring->setChecked( false );

    ui->ChBox_OptField_Manga_AltTitle->setChecked( false );
    ui->ChBox_OptField_Manga_Author->setChecked( false );
    ui->ChBox_OptField_Manga_Translation->setChecked( false );

    ui->ChBox_OptField_Dorama_AltTitle->setChecked( false );
    ui->ChBox_OptField_Dorama_Director->setChecked( false );

    ui->ComboBox_Language->setCurrentIndex(0);
    ui->ComboBox_ItemList_Sorting->setCurrentIndex(1);

    ui->ChBox_CheckForUpdate->setChecked( true );
    ui->ChBox_SwitchCoverOrDir->setChecked( true );
    ui->ChBox_SearchOnShikimori->setChecked( true );

    ui->LineEdit_WorkDir->setText( DefinesPath::appData(true) );
}

void Settings::on_PBtn_Action_Export_clicked()
{
    bool exAnime  = ui->ChBox_Export_Anime->isChecked();
    bool exManga  = ui->ChBox_Export_Manga->isChecked();
    bool exAmv    = ui->ChBox_Export_Amv->isChecked();
    bool exDorama = ui->ChBox_Export_Dorama->isChecked();
    bool exImages = ui->ChBox_Export_Images->isChecked();

    QString exportDir = ui->LineEdit_Export_FilePath->text();
            exportDir = QDir( exportDir ).path() + QDir::separator();

    if( exportDir.isEmpty() ){
        QMessageBox::warning(this, tr("Warning"), tr("The directory for export isn't choose"));
        ui->LineEdit_Export_FilePath->setFocus();
        return;
    }

    if( QDir::isAbsolutePath( exportDir ) == false ){
        QMessageBox::warning( this, tr("Warning"), tr("The path shall be absolute") );
        return;
    }

    this->setDisabled( true );

    QDir().mkpath( exportDir );
    QFile file( exportDir + "DatabaseAnime.xml" );
    if( ! file.open( QIODevice::WriteOnly | QIODevice::Text ) ){
        return;
    }
    XmlDbaWriter writer(&file);
    /*
    QSqlQuery query;
    if ( !query.exec( QString("SELECT * FROM %1").arg( MngrQuerys::getTableName(sections::anime)) ) ){
        qCritical() << QString("Cannot select data from table %1").arg( MngrQuerys::getTableName(sections::anime) );
        QMessageBox::critical(this, tr("Critical"), tr("Cannot export data") );
        return;
    }

    */
    writer.startDocument();
    writer.startElement("DatabaseAnime");
    QSqlQuery query;

    if( exAnime ){
        query = MngrQuerys::selectAll(sections::anime);
        if( query.lastError().isValid() ){
            QMessageBox::critical(this, tr("Critical"), tr("Cannot export data") );
            return;
        }

        writer.startSection(sections::anime);
        while( query.next() ){
            using namespace Tables::AnimeField;
            QMap<QString, QString> data;

            data[MngrQuerys::fieldToString(isHaveLooked)]   = query.value(MngrQuerys::fieldToString(isHaveLooked)   ).toString();
            data[MngrQuerys::fieldToString(isEditingDone)]  = query.value(MngrQuerys::fieldToString(isEditingDone)  ).toString();
            data[MngrQuerys::fieldToString(isAdult)]        = query.value(MngrQuerys::fieldToString(isAdult)        ).toString();
            data[MngrQuerys::fieldToString(Title)]          = query.value(MngrQuerys::fieldToString(Title)          ).toString();
            data[MngrQuerys::fieldToString(AltTitle)]       = query.value(MngrQuerys::fieldToString(AltTitle)       ).toString();
            data[MngrQuerys::fieldToString(Director)]       = query.value(MngrQuerys::fieldToString(Director)       ).toString();
            data[MngrQuerys::fieldToString(PostScoring)]    = query.value(MngrQuerys::fieldToString(PostScoring)    ).toString();
            data[MngrQuerys::fieldToString(SeriesTV)]       = query.value(MngrQuerys::fieldToString(SeriesTV)       ).toString();
            data[MngrQuerys::fieldToString(SeriesOVA)]      = query.value(MngrQuerys::fieldToString(SeriesOVA)      ).toString();
            data[MngrQuerys::fieldToString(SeriesONA)]      = query.value(MngrQuerys::fieldToString(SeriesONA)      ).toString();
            data[MngrQuerys::fieldToString(SeriesSpecial)]  = query.value(MngrQuerys::fieldToString(SeriesSpecial)  ).toString();
            data[MngrQuerys::fieldToString(SeriesMovie)]    = query.value(MngrQuerys::fieldToString(SeriesMovie)    ).toString();
            data[MngrQuerys::fieldToString(vSeriesTV)]      = query.value(MngrQuerys::fieldToString(vSeriesTV)      ).toString();
            data[MngrQuerys::fieldToString(vSeriesOVA)]     = query.value(MngrQuerys::fieldToString(vSeriesOVA)     ).toString();
            data[MngrQuerys::fieldToString(vSeriesONA)]     = query.value(MngrQuerys::fieldToString(vSeriesONA)     ).toString();
            data[MngrQuerys::fieldToString(vSeriesSpecial)] = query.value(MngrQuerys::fieldToString(vSeriesSpecial) ).toString();
            data[MngrQuerys::fieldToString(vSeriesMovie)]   = query.value(MngrQuerys::fieldToString(vSeriesMovie)   ).toString();
            data[MngrQuerys::fieldToString(Score)]          = query.value(MngrQuerys::fieldToString(Score)          ).toString();
            data[MngrQuerys::fieldToString(Year)]           = query.value(MngrQuerys::fieldToString(Year)           ).toString();
            data[MngrQuerys::fieldToString(Season)]         = query.value(MngrQuerys::fieldToString(Season)         ).toString();
            data[MngrQuerys::fieldToString(Studios)]        = query.value(MngrQuerys::fieldToString(Studios)        ).toString();
            data[MngrQuerys::fieldToString(Tags)]           = query.value(MngrQuerys::fieldToString(Tags)           ).toString();
            data[MngrQuerys::fieldToString(Description)]    = query.value(MngrQuerys::fieldToString(Description)    ).toString();
            data[MngrQuerys::fieldToString(Url)]            = query.value(MngrQuerys::fieldToString(Url)            ).toString();
            data[MngrQuerys::fieldToString(Dir)]            = query.value(MngrQuerys::fieldToString(Dir)            ).toString();
            data[MngrQuerys::fieldToString(ImagePath)]      = query.value(MngrQuerys::fieldToString(ImagePath)      ).toString();

            writer.writeNext(data);
            QCoreApplication::processEvents();
        }
        writer.endSection(/*Anime*/);
    }
    if( exManga ){
        query = MngrQuerys::selectAll(sections::manga);
        if( query.lastError().isValid() ){
            QMessageBox::critical(this, tr("Critical"), tr("Cannot export data") );
            return;
        }

        writer.startSection(sections::manga);
        while (query.next()) {
            using namespace Tables::MangaField;
            QMap<QString, QString> data;

            data[MngrQuerys::fieldToString(isHaveLooked)]    = query.value(MngrQuerys::fieldToString(isHaveLooked)    ).toString();
            data[MngrQuerys::fieldToString(isEditingDone)]   = query.value(MngrQuerys::fieldToString(isEditingDone)   ).toString();
            data[MngrQuerys::fieldToString(isAdult)]         = query.value(MngrQuerys::fieldToString(isAdult)         ).toString();
            data[MngrQuerys::fieldToString(Title)]           = query.value(MngrQuerys::fieldToString(Title)           ).toString();
            data[MngrQuerys::fieldToString(AltTitle)]        = query.value(MngrQuerys::fieldToString(AltTitle)        ).toString();
            data[MngrQuerys::fieldToString(Author)]          = query.value(MngrQuerys::fieldToString(Author)          ).toString();
            data[MngrQuerys::fieldToString(Translation)]     = query.value(MngrQuerys::fieldToString(Translation)     ).toString();
            data[MngrQuerys::fieldToString(Vol)]             = query.value(MngrQuerys::fieldToString(Vol)             ).toString();
            data[MngrQuerys::fieldToString(Ch)]              = query.value(MngrQuerys::fieldToString(Ch)              ).toString();
            data[MngrQuerys::fieldToString(Pages)]           = query.value(MngrQuerys::fieldToString(Pages)           ).toString();
            data[MngrQuerys::fieldToString(vVol)]            = query.value(MngrQuerys::fieldToString(vVol)            ).toString();
            data[MngrQuerys::fieldToString(vCh)]             = query.value(MngrQuerys::fieldToString(vCh)             ).toString();
            data[MngrQuerys::fieldToString(vPages)]          = query.value(MngrQuerys::fieldToString(vPages)          ).toString();
            data[MngrQuerys::fieldToString(Score)]           = query.value(MngrQuerys::fieldToString(Score)           ).toString();
            data[MngrQuerys::fieldToString(Year)]            = query.value(MngrQuerys::fieldToString(Year)            ).toString();
            data[MngrQuerys::fieldToString(Tags)]            = query.value(MngrQuerys::fieldToString(Tags)            ).toString();
            data[MngrQuerys::fieldToString(Description)]     = query.value(MngrQuerys::fieldToString(Description)     ).toString();
            data[MngrQuerys::fieldToString(Url)]             = query.value(MngrQuerys::fieldToString(Url)             ).toString();
            data[MngrQuerys::fieldToString(Dir)]             = query.value(MngrQuerys::fieldToString(Dir)             ).toString();
            data[MngrQuerys::fieldToString(ImagePath)]       = query.value(MngrQuerys::fieldToString(ImagePath)       ).toString();

            writer.writeNext(data);
            QCoreApplication::processEvents();
        }
        writer.endSection(/*Manga*/);
    }
    if( exAmv ){
        query = MngrQuerys::selectAll(sections::amv);
        if( query.lastError().isValid() ){
            QMessageBox::critical(this, tr("Critical"), tr("Cannot export data") );
            return;
        }

        writer.startSection(sections::amv);
        while (query.next()) {
            using namespace Tables::AmvField;
            QMap<QString, QString> data;

            data[MngrQuerys::fieldToString(isEditingDone)]   = query.value(MngrQuerys::fieldToString(isEditingDone)   ).toString();
            data[MngrQuerys::fieldToString(isAdult)]         = query.value(MngrQuerys::fieldToString(isAdult)         ).toString();
            data[MngrQuerys::fieldToString(Title)]           = query.value(MngrQuerys::fieldToString(Title)           ).toString();
            data[MngrQuerys::fieldToString(Author)]          = query.value(MngrQuerys::fieldToString(Author)          ).toString();
            data[MngrQuerys::fieldToString(Contestant)]      = query.value(MngrQuerys::fieldToString(Contestant)      ).toString();
            data[MngrQuerys::fieldToString(Score)]           = query.value(MngrQuerys::fieldToString(Score)           ).toString();
            data[MngrQuerys::fieldToString(Year)]            = query.value(MngrQuerys::fieldToString(Year)            ).toString();
            data[MngrQuerys::fieldToString(Tags)]            = query.value(MngrQuerys::fieldToString(Tags)            ).toString();
            data[MngrQuerys::fieldToString(ContainingMusic)] = query.value(MngrQuerys::fieldToString(ContainingMusic) ).toString();
            data[MngrQuerys::fieldToString(ContainingAnime)] = query.value(MngrQuerys::fieldToString(ContainingAnime) ).toString();
            data[MngrQuerys::fieldToString(AuthorComment)]   = query.value(MngrQuerys::fieldToString(AuthorComment)   ).toString();
            data[MngrQuerys::fieldToString(Url)]             = query.value(MngrQuerys::fieldToString(Url)             ).toString();
            data[MngrQuerys::fieldToString(Dir)]             = query.value(MngrQuerys::fieldToString(Dir)             ).toString();
            data[MngrQuerys::fieldToString(ImagePath)]       = query.value(MngrQuerys::fieldToString(ImagePath)       ).toString();

            writer.writeNext(data);
            QCoreApplication::processEvents();
        }
        writer.endSection(/*Amv*/);
    }
    if( exDorama ){
        query = MngrQuerys::selectAll(sections::dorama);
        if( query.lastError().isValid() ){
            QMessageBox::critical(this, tr("Critical"), tr("Cannot export data") );
            return;
        }

        writer.startSection(sections::dorama);
        while (query.next()) {
            using namespace Tables::DoramaField;
            QMap<QString, QString> data;

            data[MngrQuerys::fieldToString(isHaveLooked)]    = query.value(MngrQuerys::fieldToString(isHaveLooked)    ).toString();
            data[MngrQuerys::fieldToString(isEditingDone)]   = query.value(MngrQuerys::fieldToString(isEditingDone)   ).toString();
            data[MngrQuerys::fieldToString(isAdult)]         = query.value(MngrQuerys::fieldToString(isAdult)         ).toString();
            data[MngrQuerys::fieldToString(Title)]           = query.value(MngrQuerys::fieldToString(Title)           ).toString();
            data[MngrQuerys::fieldToString(AltTitle)]        = query.value(MngrQuerys::fieldToString(AltTitle)        ).toString();
            data[MngrQuerys::fieldToString(Director)]        = query.value(MngrQuerys::fieldToString(Director)        ).toString();
            data[MngrQuerys::fieldToString(SeriesTV)]        = query.value(MngrQuerys::fieldToString(SeriesTV)        ).toString();
            data[MngrQuerys::fieldToString(SeriesSpecial)]   = query.value(MngrQuerys::fieldToString(SeriesSpecial)   ).toString();
            data[MngrQuerys::fieldToString(SeriesMovie)]     = query.value(MngrQuerys::fieldToString(SeriesMovie)     ).toString();
            data[MngrQuerys::fieldToString(vSeriesTV)]       = query.value(MngrQuerys::fieldToString(vSeriesTV)       ).toString();
            data[MngrQuerys::fieldToString(vSeriesSpecial)]  = query.value(MngrQuerys::fieldToString(vSeriesSpecial)  ).toString();
            data[MngrQuerys::fieldToString(vSeriesMovie)]    = query.value(MngrQuerys::fieldToString(vSeriesMovie)    ).toString();
            data[MngrQuerys::fieldToString(Score)]           = query.value(MngrQuerys::fieldToString(Score)           ).toString();
            data[MngrQuerys::fieldToString(Year)]            = query.value(MngrQuerys::fieldToString(Year)            ).toString();
            data[MngrQuerys::fieldToString(Season)]          = query.value(MngrQuerys::fieldToString(Season)          ).toString();
            data[MngrQuerys::fieldToString(Tags)]            = query.value(MngrQuerys::fieldToString(Tags)            ).toString();
            data[MngrQuerys::fieldToString(Description)]     = query.value(MngrQuerys::fieldToString(Description)     ).toString();
            data[MngrQuerys::fieldToString(Actors)]          = query.value(MngrQuerys::fieldToString(Actors)          ).toString();
            data[MngrQuerys::fieldToString(Url)]             = query.value(MngrQuerys::fieldToString(Url)             ).toString();
            data[MngrQuerys::fieldToString(Dir)]             = query.value(MngrQuerys::fieldToString(Dir)             ).toString();
            data[MngrQuerys::fieldToString(ImagePath)]       = query.value(MngrQuerys::fieldToString(ImagePath)       ).toString();

            writer.writeNext(data);
            QCoreApplication::processEvents();
        }
        writer.endSection(/*Dorama*/);
    }
    writer.endElement(/*DatabaseAnime*/);

    file.close();

    if( exAnime  && exImages ){
        QDirIterator it( DefinesPath::animeCovers() );
        QString animeCoversExportPath( DefinesPath::animeCovers( exportDir ) );
        QDir().mkpath( animeCoversExportPath );
        while( it.hasNext() ){
            it.next();
            if( it.fileName() == "." || it.fileName() == ".." )
                continue;
            QFile( it.filePath() ).copy( animeCoversExportPath + it.fileName() );
            QCoreApplication::processEvents();
        }
    }
    if( exManga  && exImages ){
        QDirIterator it( DefinesPath::mangaCovers() );
        QString mangaCoversExportPath( DefinesPath::mangaCovers( exportDir ) );
        QDir().mkpath( mangaCoversExportPath );
        while( it.hasNext() ){
            it.next();
            if( it.fileName() == "." || it.fileName() == ".." )
                continue;
            QFile( it.filePath() ).copy( mangaCoversExportPath + it.fileName() );
            QCoreApplication::processEvents();
        }
    }
    if( exAmv && exImages ){
        QDirIterator it( DefinesPath::amvCovers() );
        QString amvCoversExportPath( DefinesPath::amvCovers( exportDir ) );
        QDir().mkpath( amvCoversExportPath );
        while( it.hasNext() ){
            it.next();
            if( it.fileName() == "." || it.fileName() == ".." )
                continue;
            QFile( it.filePath() ).copy( amvCoversExportPath + it.fileName() );
            QCoreApplication::processEvents();
        }
    }
    if( exDorama && exImages ){
        QDirIterator it( DefinesPath::doramaCovers() );
        QString doramaCoversExportPath( DefinesPath::doramaCovers( exportDir ) );
        QDir().mkpath( doramaCoversExportPath );
        while( it.hasNext() ){
            it.next();
            if( it.fileName() == "." || it.fileName() == ".." )
                continue;
            QFile( it.filePath() ).copy( doramaCoversExportPath + it.fileName() );
            QCoreApplication::processEvents();
        }
    }

    this->setEnabled( true );
    QMessageBox::information( this, tr("Export"), tr("Export is successfully finished") );
}


void Settings::on_TBtn_Export_Path_Choose_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this,
                                                    tr("Choose a directory for export file"),
                                                    ui->LineEdit_Export_FilePath->text()
                                                    );
    if( dir.isEmpty() )
        ui->LineEdit_Export_FilePath->setText( DefinesPath::home() );
    else
        ui->LineEdit_Export_FilePath->setText( dir );
}

void Settings::on_TBtn_Import_Path_Choose_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,
                                                tr("Choose a xml file for import"),
                                                ui->LineEdit_Import_FilePath->text()
                                                );
    if( file.isEmpty() )
        ui->LineEdit_Import_FilePath->setText( DefinesPath::home() );
    else
        ui->LineEdit_Import_FilePath->setText( file );
}

void Settings::on_PBtn_Import_Append_clicked()
{
    if( ui->LineEdit_Import_FilePath->text().isEmpty() ){
        QMessageBox::warning(this, tr("Warning"), tr("The imported file isn't selected"));
        ui->LineEdit_Import_FilePath->setFocus();
        this->setEnabled( true );
        return;
    }

    this->setCursor( QCursor(Qt::WaitCursor) );
    this->setDisabled(true);

    int countImportRecords = on_actionImport_triggered();
    if( countImportRecords > 0 ){
        QMessageBox::information(this, tr("Import"),"<b>" + tr("Import is successfully finished") + "</b><br>"
                                                    + tr("Records it is imported:")+ " " + QString::number(countImportRecords) + "   "
                                                        );
    }else{
        QMessageBox::critical(this, tr("Import"),"<b>" + tr("Import failed") + "</b>");
    }

    this->setEnabled(true);
    this->setCursor( QCursor(Qt::ArrowCursor) );
}

void Settings::on_PBtn_Import_Replace_clicked()
{
    QMessageBox* pmbx =
    new QMessageBox(QMessageBox::Question,
        tr("Warning"),
        tr("<b>Delete</b> your database?<br>"
           "<i>Fields marked by a checkbox will be deleted</i>"),
        QMessageBox::Yes | QMessageBox::No
    );
    int n = pmbx->exec();
    delete pmbx;

    this->setCursor( QCursor(Qt::WaitCursor) );
    this->setDisabled(true);

    if (n == QMessageBox::Yes) {
        on_actionDeleteRecords_triggered();
        on_PBtn_Import_Append_clicked();
    }

    this->setCursor( QCursor(Qt::ArrowCursor) );
    this->setEnabled(true);
}

unsigned long long Settings::on_actionImport_triggered()
{
    bool imAnime  = ui->ChBox_Import_Anime->isChecked();
    bool imManga  = ui->ChBox_Import_Manga->isChecked();
    bool imAmv    = ui->ChBox_Import_Amv->isChecked();
    bool imDorama = ui->ChBox_Import_Dorama->isChecked();
    bool imImages = ui->ChBox_Import_Images->isChecked();
    QString filePath = ui->LineEdit_Import_FilePath->text();

    QFile file( QDir(filePath).path() );
    if( ! file.open(QIODevice::ReadOnly | QIODevice::Text) ){
        qCritical() << file.errorString()
                    << "\nFileName: " << file.fileName()
                    << "\nFileError: " << file.error();
        QMessageBox::critical(this, tr("Critical"), tr("File is not open"));
        return 0;
    }

    unsigned long long n(0);
    XmlDbaReader reader(&file);
    QMap<QString, QVariant> data;

    MngrConnect.transaction();
    while( ! reader.atEnd() && ! reader.hasError() ){
        data = reader.readNext();
        if( data.isEmpty() )
            continue;

        if( reader.currentSection() == sections::anime
            && imAnime == false)
            continue;
        if( reader.currentSection() == sections::manga
            && imManga == false)
            continue;
        if( reader.currentSection() == sections::amv
            && imAmv == false)
            continue;
        if( reader.currentSection() == sections::dorama
            && imDorama == false)
            continue;

        switch ( reader.currentSection() ) {
        case sections::anime :
            MngrQuerys::insertAnime( MngrQuerys::convertAnimeData(data) );
            break;
        case sections::manga :
            MngrQuerys::insertManga( MngrQuerys::convertMangaData(data) );
            break;
        case sections::amv :
            MngrQuerys::insertAmv( MngrQuerys::convertAmvData(data) );
            break;
        case sections::dorama :
            MngrQuerys::insertDorama( MngrQuerys::convertDoramaData(data) );
            break;
        default:
            qCritical() << "[FormSettings::importAppend] uncorrect section: " << reader.currentSection();
        }
        n++;
        QCoreApplication::processEvents();
    }
    if( reader.hasError() )
        MngrConnect.rollback();
    else
        MngrConnect.commit();

    QString importPath( QFileInfo( filePath ).path() );

    if( imAnime  && imImages ){
        QDirIterator it( DefinesPath::animeCovers( importPath ) );
        QDir().mkpath( DefinesPath::animeCovers() );
        while( it.hasNext() ){
            it.next();
            if( it.fileName() == "." || it.fileName() == ".." )
                continue;
            QFile( it.filePath() ).copy( DefinesPath::animeCovers() + it.fileName() );
            QCoreApplication::processEvents();
        }
    }
    if( imManga  && imImages ){
        QDirIterator it( DefinesPath::mangaCovers( importPath ) );
        QDir().mkpath( DefinesPath::mangaCovers() );
        while( it.hasNext() ){
            it.next();
            if( it.fileName() == "." || it.fileName() == ".." )
                continue;
            QFile( it.filePath() ).copy( DefinesPath::mangaCovers() + it.fileName() );
            QCoreApplication::processEvents();
        }
    }
    if( imAmv  && imImages ){
        QDirIterator it( DefinesPath::amvCovers( importPath ) );
        QDir().mkpath( DefinesPath::amvCovers() );
        while( it.hasNext() ){
            it.next();
            if( it.fileName() == "." || it.fileName() == ".." )
                continue;
            QFile( it.filePath() ).copy( DefinesPath::amvCovers() + it.fileName() );
            QCoreApplication::processEvents();
        }
    }
    if( imDorama && imImages ){
        QDirIterator it( DefinesPath::doramaCovers( importPath ) );
        QDir().mkpath( DefinesPath::doramaCovers() );
        while( it.hasNext() ){
            it.next();
            if( it.fileName() == "." || it.fileName() == ".." )
                continue;
            QFile( it.filePath() ).copy( DefinesPath::doramaCovers() + it.fileName() );
            QCoreApplication::processEvents();
        }
    }

    file.close();

    return n;
}

bool Settings::on_actionDeleteRecords_triggered()
{
    bool imAnime  = ui->ChBox_Import_Anime->isChecked();
    bool imManga  = ui->ChBox_Import_Manga->isChecked();
    bool imAmv    = ui->ChBox_Import_Amv->isChecked();
    bool imDorama = ui->ChBox_Import_Dorama->isChecked();
    bool imImages = ui->ChBox_Import_Images->isChecked();

    MngrConnect.transaction();
    if( imAnime ){
        if( MngrQuerys::dropTable( sections::anime ) == false ){
            MngrConnect.rollback();
            return false;
        }
    }
    if( imManga ){
        if( MngrQuerys::dropTable( sections::manga ) == false ){
            MngrConnect.rollback();
            return false;
        }
    }
    if( imAmv ){
        if( MngrQuerys::dropTable( sections::amv ) == false ){
            MngrConnect.rollback();
            return false;
        }
    }
    if( imDorama ){
        if( MngrQuerys::dropTable( sections::dorama ) == false ){
            MngrConnect.rollback();
            return false;
        }
    }

    if( imImages ){
        if( imAnime ){
            QDirIterator it( DefinesPath::animeCovers() );
            while( it.hasNext() ){
                it.next();
                if( it.fileName() == "." || it.fileName() == ".." )
                    continue;
                QFile( it.filePath() ).remove();
                QCoreApplication::processEvents();
            }
        }

        if( imManga ){
            QDirIterator it( DefinesPath::mangaCovers() );
            while( it.hasNext() ){
                it.next();
                if( it.fileName() == "." || it.fileName() == ".." )
                    continue;
                QFile( it.filePath() ).remove();
                QCoreApplication::processEvents();
            }
        }

        if( imAmv ){
            QDirIterator it( DefinesPath::amvCovers() );
            while( it.hasNext() ){
                it.next();
                if( it.fileName() == "." || it.fileName() == ".." )
                    continue;
                QFile( it.filePath() ).remove();
                QCoreApplication::processEvents();
            }
        }

        if( imDorama ){
            QDirIterator it( DefinesPath::doramaCovers() );
            while( it.hasNext() ){
                it.next();
                if( it.fileName() == "." || it.fileName() == ".." )
                    continue;
                QFile( it.filePath() ).remove();
                QCoreApplication::processEvents();
            }
        }
    }

    MngrQuerys::createTable_Anime();
    MngrQuerys::createTable_Manga();
    MngrQuerys::createTable_Amv();
    MngrQuerys::createTable_Dorama();
    MngrConnect.commit();
    return true;
}

void Settings::on_TBtn_WorkDir_Choose_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this,
                                                    tr("Choose a directory for application data"),
                                                    ui->LineEdit_WorkDir->text()
                                                    );
    if( dir.isEmpty() )
        ui->LineEdit_WorkDir->setText( DefinesPath::appData() );
    else
        ui->LineEdit_WorkDir->setText( dir );
}
