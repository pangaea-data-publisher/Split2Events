#ifndef SPLIT2EVENTSDIALOG_H
#define SPLIT2EVENTSDIALOG_H

#include <QDialog>

#include "ui_split2eventsdialog.h"

class Split2EventsDialog : public QDialog, public Ui::Split2EventsDialog
{
    Q_OBJECT

public:
    Split2EventsDialog( QWidget *parent = 0 );

    QString makeNumberList( const QString s_NumberList );
    int readFile( const QString &FilenameIn, QStringList &Input, const int Codec = -1, const qint64 i_Bytes = 0 );
    void setLineEditText();

private slots:
    void NewProject();
    void SaveProject();
    void LoadProject();
    void enableOKButton();
    void CreateMetadataTemplatePushButtonClicked();
};

#endif
