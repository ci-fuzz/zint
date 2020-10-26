/***************************************************************************
 *   Copyright (C) 2008 by BogDan Vatra <bogdan@licentia.eu>               *
 *   Copyright (C) 2009-2020 by Robin Stuart <rstuart114@gmail.com>        *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/
/* vim: set ts=4 sw=4 et : */

//#include <QDebug>
#include <QGraphicsScene>
#include <QImage>
#include <QColorDialog>
#include <QUiLoader>
#include <QFile>
#include <QRadioButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QClipboard>
#include <QMimeData>
#include <QColor>
#include <QListView>
#include <QShortcut>

#include "mainwindow.h"
#include "datawindow.h"
#include "sequencewindow.h"
#include <stdio.h>

MainWindow::MainWindow(QWidget* parent, Qt::WindowFlags fl)
        : QWidget(parent, fl), m_optionWidget(NULL), m_symbology(0)
{
    m_bc.bc.setDebug(QCoreApplication::arguments().contains("--verbose")); // Undocumented command line debug flag

    QCoreApplication::setOrganizationName("zint");
    QCoreApplication::setOrganizationDomain("zint.org.uk");
    QCoreApplication::setApplicationName("Barcode Studio");

    QSettings settings;

    char bstyle_text[][50] = {
        "Australia Post Redirect Code",
        "Australia Post Reply-Paid",
        "Australia Post Routing Code",
        "Australia Post Standard Customer",
        "Aztec Code (ISO 24778) (and HIBC)",
        "Aztec Runes",
        "Channel Code",
        "Codabar",
        "Codablock-F (and HIBC)",
        "Code 11",
        "Code 128 (ISO 15417) (and GS1-128 and HIBC)",
        "Code 16k",
        "Code 2 of 5 Data Logic",
        "Code 2 of 5 IATA",
        "Code 2 of 5 Industrial",
        "Code 2 of 5 Interleaved",
        "Code 2 of 5 Standard (Matrix)",
        "Code 32 (Italian Pharmacode)",
        "Code 39 (ISO 16388) (and HIBC)",
        "Code 39 Extended",
        "Code 49",
        "Code 93",
        "Code One",
        "DAFT Code",
        "Data Matrix (ISO 16022) (and HIBC)",
        "Deutsche Post Identcode",
        "Deutsche Post Leitcode",
        "DotCode",
        "DPD Code",
        "Dutch Post KIX",
        "EAN-14",
        "European Article Number (EAN)",
        "Facing Identification Mark (FIM)",
        "Flattermarken",
        "Grid Matrix",
        "GS1 DataBar Expanded",
        "GS1 DataBar Expanded Stacked",
        "GS1 DataBar Limited",
        "GS1 DataBar Omnidirectional (and Truncated)",
        "GS1 DataBar Stacked",
        "GS1 DataBar Stacked Omnidirectional",
        "Han Xin (Chinese Sensible) Code",
        "International Standard Book Number (ISBN)",
        "ITF-14",
        "Japanese Postal Barcode",
        "Korean Postal Barcode",
        "LOGMARS",
        "Maxicode (ISO 16023)",
        "MicroPDF417 (ISO 24728) (and HIBC)",
        "Micro QR Code",
        "MSI Plessey",
        "NVE-18 (SSCC-18)",
        "PDF417 (ISO 15438) (and Compact and HIBC)",
        "Pharmacode",
        "Pharmacode 2-track",
        "Pharma Zentralnummer (PZN)",
        "PLANET",
        "POSTNET",
        "QR Code (ISO 18004) (and HIBC)",
        "Rectangular Micro QR (rMQR)",
        "Royal Mail 4-state Barcode",
        "Royal Mail 4-state Mailmark",
        "Telepen",
        "Telepen Numeric",
        "UK Plessey",
        "Ultracode",
        "Universal Product Code (UPC-A)",
        "Universal Product Code (UPC-E)",
        "UPNQR",
        "USPS Intelligent Mail (OneCode)",
        "VIN (Vehicle Identification Number)"
    };

    scene = new QGraphicsScene(this);

    setupUi(this);
    view->setScene(scene);

    restoreGeometry(settings.value("studio/window_geometry").toByteArray());

    m_fgcolor.setRgb(settings.value("studio/ink/red", 0).toInt(),
                    settings.value("studio/ink/green", 0).toInt(),
                    settings.value("studio/ink/blue", 0).toInt(),
                    settings.value("studio/ink/alpha", 0xff).toInt());
    m_bgcolor.setRgb(settings.value("studio/paper/red", 0xff).toInt(),
                    settings.value("studio/paper/green", 0xff).toInt(),
                    settings.value("studio/paper/blue", 0xff).toInt(),
                    settings.value("studio/paper/alpha", 0xff).toInt());

    int cnt = metaObject()->enumerator(0).keyCount();
    for (int i = 0; i < cnt; i++) {
        bstyle->addItem(metaObject()->enumerator(0).key(i));
        bstyle->setItemText(i, bstyle_text[i]);
    }
#ifdef _WIN32
    bstyle->setMaxVisibleItems(cnt); /* Apart from increasing combo size, seems to be needed for filter to work */
#endif
#if QT_VERSION <  0x50A00
    /* Prior to Qt 5.10 comboboxes have display issues when filtered (scrollers not accounted for), so disable */
    filter_bstyle->hide();
#endif

    bstyle->setCurrentIndex(settings.value("studio/symbology", 10).toInt());

    txtData->setText(settings.value("studio/data", "Your Data Here!").toString());
    txtComposite->setText(settings.value("studio/composite_text", "Your Data Here!").toString());
    chkComposite->setChecked(settings.value("studio/chk_composite").toInt() ? true : false);
    cmbCompType->setCurrentIndex(settings.value("studio/comp_type", 0).toInt());
    cmbECI->setCurrentIndex(settings.value("studio/appearance/eci", 0).toInt());
    chkEscape->setChecked(settings.value("studio/chk_escape").toInt() ? true : false);
    chkData->setChecked(settings.value("studio/chk_data").toInt() ? true : false);
    chkRInit->setChecked(settings.value("studio/chk_rinit").toInt() ? true : false);
    chkAutoHeight->setChecked(settings.value("studio/appearance/autoheight", 1).toInt() ? true : false);
    heightb->setValue(settings.value("studio/appearance/height", 50).toInt());
    bwidth->setValue(settings.value("studio/appearance/border", 0).toInt());
    spnWhitespace->setValue(settings.value("studio/appearance/whitespace", 0).toInt());
    spnScale->setValue(settings.value("studio/appearance/scale", 1.0).toFloat());
    btype->setCurrentIndex(settings.value("studio/appearance/border_type", 0).toInt());
    cmbFontSetting->setCurrentIndex(settings.value("studio/appearance/font_setting", 0).toInt());
    chkHRTShow->setChecked(settings.value("studio/appearance/chk_hrt_show", 1).toInt() ? true : false);
    chkCMYK->setChecked(settings.value("studio/appearance/cmyk", 0).toInt() ? true : false);
    cmbRotate->setCurrentIndex(settings.value("studio/appearance/rotate", 0).toInt());
    chkDotty->setChecked(settings.value("studio/appearance/dotty", 0).toInt() ? true : false);
    spnDotSize->setValue(settings.value("studio/appearance/dot_size", 4.0 / 5.0).toFloat());

    change_options();

    scene->addItem(&m_bc);

    update_preview();

    connect(bstyle, SIGNAL(currentIndexChanged( int )), SLOT(change_options()));
    connect(bstyle, SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
    connect(filter_bstyle, SIGNAL(textChanged( const QString& )), SLOT(filter_symbologies()));
    connect(heightb, SIGNAL(valueChanged( int )), SLOT(update_preview()));
    connect(bwidth,  SIGNAL(valueChanged( int )), SLOT(update_preview()));
    connect(btype, SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
    connect(cmbFontSetting, SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
    connect(txtData, SIGNAL(textChanged( const QString& )), SLOT(update_preview()));
    connect(txtComposite, SIGNAL(textChanged()), SLOT(update_preview()));
    connect(chkComposite, SIGNAL(stateChanged( int )), SLOT(composite_ui_set()));
    connect(chkComposite, SIGNAL(stateChanged( int )), SLOT(update_preview()));
    connect(cmbCompType, SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
    connect(cmbECI, SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
    connect(chkEscape, SIGNAL(stateChanged( int )), SLOT(update_preview()));
    connect(chkData, SIGNAL(stateChanged( int )), SLOT(update_preview()));
    connect(chkRInit, SIGNAL(stateChanged( int )), SLOT(update_preview()));
    connect(spnWhitespace, SIGNAL(valueChanged( int )), SLOT(update_preview()));
    connect(btnAbout, SIGNAL(clicked( bool )), SLOT(about()));
    connect(btnSave, SIGNAL(clicked( bool )), SLOT(save()));
    connect(spnScale, SIGNAL(valueChanged( double )), SLOT(change_print_scale()));
    connect(btnExit, SIGNAL(clicked( bool )), SLOT(quit_now()));
    connect(btnReset, SIGNAL(clicked( bool )), SLOT(reset_view()));
    connect(btnMoreData, SIGNAL(clicked( bool )), SLOT(open_data_dialog()));
    connect(btnSequence, SIGNAL(clicked( bool )), SLOT(open_sequence_dialog()));
    connect(chkAutoHeight, SIGNAL(stateChanged( int )), SLOT(autoheight_ui_set()));
    connect(chkAutoHeight, SIGNAL(stateChanged( int )), SLOT(update_preview()));
    connect(chkHRTShow, SIGNAL(stateChanged( int )), SLOT(HRTShow_ui_set()));
    connect(chkHRTShow, SIGNAL(stateChanged( int )), SLOT(update_preview()));
    connect(chkCMYK, SIGNAL(stateChanged( int )), SLOT(change_cmyk()));
    connect(cmbRotate, SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
    connect(chkDotty, SIGNAL(stateChanged( int )), SLOT(dotty_ui_set()));
    connect(chkDotty, SIGNAL(stateChanged( int )), SLOT(update_preview()));
    connect(spnDotSize, SIGNAL(valueChanged( double )), SLOT(update_preview()));
    connect(btnCopySVG, SIGNAL(clicked( bool )), SLOT(copy_to_clipboard_svg()));
    connect(btnCopyBMP, SIGNAL(clicked( bool )), SLOT(copy_to_clipboard_bmp()));

    connect(&m_bc.bc, SIGNAL(encoded()), SLOT(on_encoded()));

    QShortcut *ctrl_q = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q), this);
    connect(ctrl_q, SIGNAL(activated()), SLOT(quit_now()));
}

MainWindow::~MainWindow()
{
    QSettings settings;

    settings.setValue("studio/window_geometry", saveGeometry());
    settings.setValue("studio/tab_index", tabMain->currentIndex());
    settings.setValue("studio/symbology", bstyle->currentIndex());
    settings.setValue("studio/ink/red", m_fgcolor.red());
    settings.setValue("studio/ink/green", m_fgcolor.green());
    settings.setValue("studio/ink/blue", m_fgcolor.blue());
    settings.setValue("studio/ink/alpha", m_fgcolor.alpha());
    settings.setValue("studio/paper/red", m_bgcolor.red());
    settings.setValue("studio/paper/green", m_bgcolor.green());
    settings.setValue("studio/paper/blue", m_bgcolor.blue());
    settings.setValue("studio/paper/alpha", m_bgcolor.alpha());
    settings.setValue("studio/data", txtData->text());
    settings.setValue("studio/composite_text", txtComposite->toPlainText());
    settings.setValue("studio/chk_composite", chkComposite->isChecked() ? 1 : 0);
    settings.setValue("studio/comp_type", cmbCompType->currentIndex());
    settings.setValue("studio/eci", cmbECI->currentIndex());
    settings.setValue("studio/chk_escape", chkEscape->isChecked() ? 1 : 0);
    settings.setValue("studio/chk_data", chkData->isChecked() ? 1 : 0);
    settings.setValue("studio/chk_rinit", chkRInit->isChecked() ? 1 : 0);
    settings.setValue("studio/appearance/autoheight", chkAutoHeight->isChecked() ? 1 : 0);
    settings.setValue("studio/appearance/height", heightb->value());
    settings.setValue("studio/appearance/border", bwidth->value());
    settings.setValue("studio/appearance/whitespace", spnWhitespace->value());
    settings.setValue("studio/appearance/scale", spnScale->value());
    settings.setValue("studio/appearance/border_type", btype->currentIndex());
    settings.setValue("studio/appearance/font_setting", cmbFontSetting->currentIndex());
    settings.setValue("studio/appearance/chk_hrt_show", chkHRTShow->isChecked() ? 1 : 0);
    settings.setValue("studio/appearance/chk_cmyk", chkCMYK->isChecked() ? 1 : 0);
    settings.setValue("studio/appearance/rotate", cmbRotate->currentIndex());
    settings.setValue("studio/appearance/chk_dotty", chkDotty->isChecked() ? 1 : 0);
    settings.setValue("studio/appearance/dot_size", spnDotSize->value());

    save_sub_settings(settings, m_bc.bc.symbol());
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    update_preview();
}

void MainWindow::reset_view()
{
    m_fgcolor.setRgb(0, 0, 0, 0xff);
    m_bgcolor.setRgb(0xff, 0xff, 0xff, 0xff);
    update_preview();
}

bool MainWindow::save()
{
    QSettings settings;
    QFileDialog save_dialog;
    QString filename;
    QString suffix;

    save_dialog.setAcceptMode(QFileDialog::AcceptSave);
    save_dialog.setWindowTitle("Save Barcode Image");
    save_dialog.setDirectory(settings.value("studio/default_dir", QDir::toNativeSeparators(QDir::homePath())).toString());

#ifdef NO_PNG
    suffix = settings.value("studio/default_suffix", "gif").toString();
    save_dialog.setNameFilter(tr("Encapsulated PostScript (*.eps);;Graphics Interchange Format (*.gif);;Scalable Vector Graphic (*.svg);;Windows Bitmap (*.bmp);;ZSoft PC Painter Image (*.pcx);;Enhanced Metafile (*.emf);;Tagged Image File Format (*.tif)"));
#else
    suffix = settings.value("studio/default_suffix", "png").toString();
    save_dialog.setNameFilter(tr("Portable Network Graphic (*.png);;Encapsulated PostScript (*.eps);;Graphics Interchange Format (*.gif);;Scalable Vector Graphic (*.svg);;Windows Bitmap (*.bmp);;ZSoft PC Painter Image (*.pcx);;Enhanced Metafile (*.emf);;Tagged Image File Format (*.tif)"));
#endif

    if (QString::compare(suffix, "png", Qt::CaseInsensitive) == 0)
        save_dialog.selectNameFilter("Portable Network Graphic (*.png)");
    if (QString::compare(suffix, "eps", Qt::CaseInsensitive) == 0)
        save_dialog.selectNameFilter("Encapsulated PostScript (*.eps)");
    if (QString::compare(suffix, "gif", Qt::CaseInsensitive) == 0)
        save_dialog.selectNameFilter("Graphics Interchange Format (*.gif)");
    if (QString::compare(suffix, "svg", Qt::CaseInsensitive) == 0)
        save_dialog.selectNameFilter("Scalable Vector Graphic (*.svg)");
    if (QString::compare(suffix, "bmp", Qt::CaseInsensitive) == 0)
        save_dialog.selectNameFilter("Windows Bitmap (*.bmp)");
    if (QString::compare(suffix, "pcx", Qt::CaseInsensitive) == 0)
        save_dialog.selectNameFilter("ZSoft PC Painter Image (*.pcx)");
    if (QString::compare(suffix, "emf", Qt::CaseInsensitive) == 0)
        save_dialog.selectNameFilter("Enhanced Metafile (*.emf)");
    if (QString::compare(suffix, "tif", Qt::CaseInsensitive) == 0)
        save_dialog.selectNameFilter("Tagged Image File Format (*.tif)");

    if (save_dialog.exec()) {
        filename = save_dialog.selectedFiles().at(0);
        if ((filename.lastIndexOf(".") == -1) || (filename.lastIndexOf(".") < (filename.length() - 5))) {
            suffix = save_dialog.selectedNameFilter();
            suffix = suffix.mid((suffix.lastIndexOf(".") + 1), 3);
            filename.append(".");
            filename.append(suffix);
        } else {
            suffix = filename.right(filename.length() - (filename.lastIndexOf('.') + 1));
        }
    } else {
        return false;
    }

    if(m_bc.bc.save_to_file(filename) == false) {
        if (m_bc.bc.getError() >= ZINT_ERROR) {
            QMessageBox::critical(this, tr("Save Error"), m_bc.bc.error_message());
            return false;
        } else {
            QMessageBox::warning(this, tr("Save Warning"), m_bc.bc.error_message());
            return true;
        }
    }

    settings.setValue("studio/default_dir", filename.mid(0, filename.lastIndexOf(QDir::separator())));
    settings.setValue("studio/default_suffix", suffix);
    return true;
}

void MainWindow::about()
{
    QString zint_version;
    if (ZINT_VERSION_BUILD) {
        QTextStream(&zint_version) << ZINT_VERSION_MAJOR << "." << ZINT_VERSION_MINOR << "." << ZINT_VERSION_RELEASE << "." << ZINT_VERSION_BUILD;
    } else {
        QTextStream(&zint_version) << ZINT_VERSION_MAJOR << "." << ZINT_VERSION_MINOR << "." << ZINT_VERSION_RELEASE;
    }

    QMessageBox::about(this, tr("About Zint"),
        /*: %1 is Zint version, %2 is Qt version */
        tr("<h2>Zint Barcode Studio %1</h2>"
           "<p>A free barcode generator"
           "<p>Instruction manual is available at the project homepage:<br>"
           "<a href=\"http://www.zint.org.uk\">http://www.zint.org.uk</a>"
           "<p>Copyright &copy; 2006-2020 Robin Stuart and others.<br>"
           "Qt back end by BogDan Vatra<br>"
           "Windows port by Harald Oehlmann</p>"
           "<p>Qt version %2"
           "<p>With thanks to Norbert Szab&oacute;, Robert Elliott, "
               "Milton Neal, Git Lost and many others at Sourceforge."
           "<p>Released under the GNU General Public License ver. 3 or later.<br>"
           "\"QR Code\" is a Registered Trademark of Denso Corp.<br>"
           "\"Telepen\" is a Registered Trademark of SB Electronics."
           "<p><table border=1><tr><td><small>Currently supported standards include:<br>"
           "BS 797:1996, BS 798:1996, ISO/IEC 12323:2005, ISO/IEC 15417:2007,<br>"
           "ISO/IEC 15438:2015, ISO/IEC 16022:2006, ISO/IEC 16023:2000,<br>"
           "ISO/IEC 16388:2007, ISO/IEC 18004:2015, ISO/IEC 24723:2010,<br>"
           "ISO/IEC 24724:2011, ISO/IEC 24728:2006, ISO/IEC 24778:2008,<br>"
           "ISO/IEC 21471:2019, ANSI-HIBC 2.6-2016, ANSI/AIM BC6-2000,<br>"
           "ANSI/AIM BC12-1998, AIMD014 (v 1.63), USPS-B-3200"
           "</small></td></tr></table>").arg(zint_version).arg(QT_VERSION_STR));
}

int MainWindow::open_data_dialog()
{
    int retval;
    DataWindow dlg(txtData->text());
    retval = dlg.exec();
    if (dlg.Valid == 1)
        txtData->setText(dlg.DataOutput);
    return retval;
}

int MainWindow::open_sequence_dialog()
{
    SequenceWindow dlg;
    dlg.barcode = &m_bc;
    return dlg.exec();
}

void MainWindow::on_fgcolor_clicked()
{
    QColor temp = m_fgcolor;
    m_fgcolor=QColorDialog::getColor(m_fgcolor,this,"Set foreground colour",QColorDialog::ShowAlphaChannel);
    if (m_fgcolor.isValid()) {
        update_preview();
    } else {
        m_fgcolor = temp;
    }
}

void MainWindow::on_bgcolor_clicked()
{
    QColor temp = m_bgcolor;
    m_bgcolor=QColorDialog::getColor(m_bgcolor,this,"Set background colour",QColorDialog::ShowAlphaChannel);
    if (m_bgcolor.isValid()) {
        update_preview();
    } else {
        m_bgcolor = temp;
    }
}

void MainWindow::autoheight_ui_set()
{
    if (!chkAutoHeight->isEnabled() || chkAutoHeight->isChecked()) {
        lblHeight->setEnabled(false);
        heightb->setEnabled(false);
    } else {
        lblHeight->setEnabled(true);
        heightb->setEnabled(true);
    }
}

void MainWindow::HRTShow_ui_set()
{
    if (chkHRTShow->isEnabled() && chkHRTShow->isChecked()) {
        lblFontSetting->setEnabled(true);
        cmbFontSetting->setEnabled(true);
    } else {
        lblFontSetting->setEnabled(false);
        cmbFontSetting->setEnabled(false);
    }
}

void MainWindow::dotty_ui_set()
{
    int symbology = metaObject()->enumerator(0).value(bstyle->currentIndex());

    if (symbology == BARCODE_DOTCODE) {
        chkDotty->setEnabled(false);
        lblDotSize->setEnabled(true);
        spnDotSize->setEnabled(true);
    } else {
        if (chkDotty->isEnabled() && chkDotty->isChecked()) {
            lblDotSize->setEnabled(true);
            spnDotSize->setEnabled(true);
        } else {
            lblDotSize->setEnabled(false);
            spnDotSize->setEnabled(false);
        }
    }
}

void MainWindow::on_encoded()
{
    if (!chkAutoHeight->isEnabled() || chkAutoHeight->isChecked()) {
        heightb->setValue(m_bc.bc.height());
    }
}

void MainWindow::filter_symbologies()
{
    QString filter = filter_bstyle->text().simplified(); /* `simplified()` trims and reduces inner whitespace to a single space - nice! */
    QListView *view = qobject_cast<QListView *>(bstyle->view());
    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(bstyle->model());
    QStandardItem *item;

    if (!view || !model) {
        return;
    }

    /* QString::split() only introduced Qt 5.14, so too new for us to use */
    QStringList filter_list;
    if (!filter.isEmpty()) {
        int i, j;
        for (i = 0; (j = filter.indexOf(" ", i)) != -1; i = j + 1) {
            filter_list << filter.mid(i, j - i);
        }
        filter_list << filter.mid(i);
    }
    int filter_cnt = filter_list.size();
    int cnt = metaObject()->enumerator(0).keyCount();

    if (filter_cnt) {
        for (int i = 0; i < cnt; i++) {
            bool hidden = view->isRowHidden(i);
            bool hide = true;
            for (int j = 0; j < filter_cnt; j++) {
                if (bstyle->itemText(i).contains(filter_list[j], Qt::CaseInsensitive)) {
                    hide = false;
                    break;
                }
            }
            if ((hide && !hidden) || (!hide && hidden)) {
                // https://stackoverflow.com/questions/25172220/how-to-hide-qcombobox-items-instead-of-clearing-them-out
                item = model->item(i);
                item->setFlags(hide ? item->flags() & ~Qt::ItemIsEnabled : item->flags() | Qt::ItemIsEnabled);
                view->setRowHidden(i, hide);
            }
        }
    } else {
        for (int i = 0; i < cnt; i++) {
            if (view->isRowHidden(i)) {
                item = model->item(i);
                item->setFlags(item->flags() | Qt::ItemIsEnabled);
                view->setRowHidden(i, false);
            }
        }
    }
}

void MainWindow::change_print_scale()
{
    /* This value is only used when printing (saving) to file */
    m_bc.bc.setScale((float)spnScale->value());
}

void MainWindow::change_cmyk()
{
    /* This value is only used when printing (saving) to file */
    m_bc.bc.setCMYK(chkCMYK->isChecked());
}

void MainWindow::quit_now()
{
    close();
}

void MainWindow::copy_to_clipboard_svg()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString filename = ".zint.svg";
    double scale = spnScale->value();

    spnScale->setValue(5);

    if (!m_bc.bc.save_to_file(filename)) {
        spnScale->setValue(scale);
        return;
    }

    QMimeData *data = new QMimeData;
    data->setImageData(QImage(filename));
    clipboard->setMimeData(data, QClipboard::Clipboard);

    QFile::remove(filename);

    spnScale->setValue(scale);
}

void MainWindow::copy_to_clipboard_bmp()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString filename = ".zint.bmp";

    if (!m_bc.bc.save_to_file(filename)) {
        return;
    }

    QMimeData *data = new QMimeData;
    data->setImageData(QImage(filename));
    clipboard->setMimeData(data, QClipboard::Clipboard);

    QFile::remove(filename);
}

void MainWindow::change_options()
{
    QUiLoader uiload;
    QSettings settings;

    bool initial_load = m_symbology == 0;
    int original_tab_count = tabMain->count();
    int original_tab_index = tabMain->currentIndex();
    int symbology = metaObject()->enumerator(0).value(bstyle->currentIndex());

    if (m_symbology) {
        save_sub_settings(settings, m_symbology);
    }

    if (tabMain->count()==3)
        tabMain->removeTab(1);

    chkComposite->setText(tr("Add &2D Component"));
    btype->setItemText(0, tr("No border"));
    combobox_item_enabled(cmbFontSetting, 1, true);
    cmbFontSetting->setItemText(2, tr("Small"));

    if (symbology == BARCODE_CODE128)
    {
        QFile file(":/grpC128.ui");
        if (!file.open(QIODevice::ReadOnly))
            return;
        m_optionWidget=uiload.load(&file);
        file.close();
        tabMain->insertTab(1,m_optionWidget,tr("Cod&e 128"));
        chkComposite->setText(tr("Add &2D Component (GS1-128 only)"));
        connect(m_optionWidget->findChild<QObject*>("radC128EAN"), SIGNAL(toggled( bool )), SLOT(composite_ean_check()));
        connect(m_optionWidget->findChild<QObject*>("radC128Stand"), SIGNAL(clicked( bool )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("radC128CSup"), SIGNAL(clicked( bool )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("radC128EAN"), SIGNAL(clicked( bool )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("radC128HIBC"), SIGNAL(clicked( bool )), SLOT(update_preview()));
    }

    if (symbology == BARCODE_PDF417)
    {
        QFile file(":/grpPDF417.ui");
        if (!file.open(QIODevice::ReadOnly))
            return;
        m_optionWidget=uiload.load(&file);
        file.close();
        tabMain->insertTab(1,m_optionWidget,tr("PDF41&7"));
        connect(m_optionWidget->findChild<QObject*>("cmbPDFECC"), SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("cmbPDFCols"), SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("radPDFTruncated"), SIGNAL(clicked( bool )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("radPDFStand"), SIGNAL(clicked( bool )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("radPDFHIBC"), SIGNAL(clicked( bool )), SLOT(update_preview()));

    }

    if (symbology == BARCODE_MICROPDF417)
    {
        QFile file(":/grpMicroPDF.ui");
        if (!file.open(QIODevice::ReadOnly))
            return;
        m_optionWidget=uiload.load(&file);
        file.close();
        tabMain->insertTab(1,m_optionWidget,tr("Micro PDF41&7"));
        connect(m_optionWidget->findChild<QObject*>("cmbMPDFCols"), SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("radMPDFStand"), SIGNAL(toggled( bool )), SLOT(update_preview()));
    }

    if (symbology == BARCODE_DOTCODE)
    {
        QFile file(":/grpDotCode.ui");
        if (!file.open(QIODevice::ReadOnly))
            return;
        m_optionWidget=uiload.load(&file);
        file.close();
        tabMain->insertTab(1,m_optionWidget,tr("DotCod&e"));
        connect(m_optionWidget->findChild<QObject*>("cmbDotCols"), SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("radDotStand"), SIGNAL(clicked( bool )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("radDotGS1"), SIGNAL(clicked( bool )), SLOT(update_preview()));
    }

    if (symbology == BARCODE_AZTEC)
    {
        QFile file(":/grpAztec.ui");
        if (!file.open(QIODevice::ReadOnly))
            return;
        m_optionWidget=uiload.load(&file);
        file.close();
        tabMain->insertTab(1,m_optionWidget,tr("Aztec Cod&e"));
        connect(m_optionWidget->findChild<QObject*>("radAztecAuto"), SIGNAL(clicked( bool )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("radAztecSize"), SIGNAL(clicked( bool )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("radAztecECC"), SIGNAL(clicked( bool )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("cmbAztecSize"), SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("cmbAztecECC"), SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("radAztecStand"), SIGNAL(clicked( bool )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("radAztecGS1"), SIGNAL(clicked( bool )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("radAztecHIBC"), SIGNAL(clicked( bool )), SLOT(update_preview()));
    }

    if (symbology == BARCODE_MSI_PLESSEY)
    {
        QFile file(":/grpMSICheck.ui");
        if (!file.open(QIODevice::ReadOnly))
            return;
        m_optionWidget=uiload.load(&file);
        file.close();
        tabMain->insertTab(1,m_optionWidget,tr("MSI Pless&ey"));
        connect(m_optionWidget->findChild<QObject*>("cmbMSICheck"), SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
    }

    if (symbology == BARCODE_CODE11)
    {
        QFile file(":/grpC11.ui");
        if (!file.open(QIODevice::ReadOnly))
            return;
        m_optionWidget=uiload.load(&file);
        file.close();
        tabMain->insertTab(1,m_optionWidget,tr("Cod&e 11"));
        connect(m_optionWidget->findChild<QObject*>("radC11TwoCheckDigits"), SIGNAL(clicked( bool )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("radC11OneCheckDigit"), SIGNAL(clicked( bool )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("radC11NoCheckDigits"), SIGNAL(clicked( bool )), SLOT(update_preview()));
    }

    if ((symbology == BARCODE_CODE39) || (symbology == BARCODE_EXCODE39))
    {
        QFile file(":/grpC39.ui");
        if (!file.open(QIODevice::ReadOnly))
            return;
        m_optionWidget=uiload.load(&file);
        file.close();
        connect(m_optionWidget->findChild<QObject*>("radC39Stand"), SIGNAL(clicked( bool )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("radC39Check"), SIGNAL(clicked( bool )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("radC39HIBC"), SIGNAL(clicked( bool )), SLOT(update_preview()));
        if (symbology == BARCODE_EXCODE39)
        {
            tabMain->insertTab(1,m_optionWidget,tr("Cod&e 39 Extended"));
            if(m_optionWidget->findChild<QRadioButton*>("radC39HIBC")->isChecked() == true)
            {
                m_optionWidget->findChild<QRadioButton*>("radC39HIBC")->setChecked(false);
                m_optionWidget->findChild<QRadioButton*>("radC39Stand")->setChecked(true);
            }
            m_optionWidget->findChild<QRadioButton*>("radC39HIBC")->setEnabled(false);
        }
        else {
            tabMain->insertTab(1,m_optionWidget,tr("Cod&e 39"));
            m_optionWidget->findChild<QRadioButton*>("radC39HIBC")->setEnabled(true);
        }
    }

    if (symbology == BARCODE_LOGMARS)
    {
        QFile file(":/grpLOGMARS.ui");
        if (!file.open(QIODevice::ReadOnly))
            return;
        m_optionWidget=uiload.load(&file);
        file.close();
        tabMain->insertTab(1,m_optionWidget,tr("LOGMARS"));
        connect(m_optionWidget->findChild<QObject*>("radLOGMARSStand"), SIGNAL(clicked( bool )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("radLOGMARSCheck"), SIGNAL(clicked( bool )), SLOT(update_preview()));
    }

    if (symbology == BARCODE_CODE16K)
    {
        QFile file(":/grpC16k.ui");
        if (!file.open(QIODevice::ReadOnly))
            return;
        m_optionWidget=uiload.load(&file);
        file.close();
        tabMain->insertTab(1,m_optionWidget,tr("Cod&e 16K"));
        btype->setItemText(0, tr("Default (bind)"));
        connect(m_optionWidget->findChild<QObject*>("cmbC16kRowSepHeight"), SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("radC16kStand"), SIGNAL(toggled( bool )), SLOT(update_preview()));
    }

    if (symbology == BARCODE_CODABAR)
    {
        QFile file(":/grpCodabar.ui");
        if (!file.open(QIODevice::ReadOnly))
            return;
        m_optionWidget=uiload.load(&file);
        file.close();
        tabMain->insertTab(1,m_optionWidget,tr("Codabar"));
        connect(m_optionWidget->findChild<QObject*>("chkCodabarCheck"), SIGNAL(clicked( bool )), SLOT(update_preview()));
    }

    if (symbology == BARCODE_CODABLOCKF)
    {
        QFile file (":/grpCodablockF.ui");
        if (!file.open(QIODevice::ReadOnly))
            return;
        m_optionWidget=uiload.load(&file);
        file.close();
        tabMain->insertTab(1,m_optionWidget,tr("Codablock&-F"));
        btype->setItemText(0, tr("Default (bind)"));
        connect(m_optionWidget->findChild<QObject*>("cmbCbfWidth"), SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("cmbCbfHeight"), SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("cmbCbfRowSepHeight"), SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("radCbfStand"), SIGNAL(clicked( bool )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("radCbfHIBC"), SIGNAL(clicked( bool )), SLOT(update_preview()));
    }

    if (symbology == BARCODE_DATAMATRIX)
    {
        QFile file(":/grpDM.ui");
        if (!file.open(QIODevice::ReadOnly))
            return;
        m_optionWidget=uiload.load(&file);
        file.close();
        tabMain->insertTab(1,m_optionWidget,tr("Data Matrix"));
        connect(m_optionWidget->findChild<QObject*>("radDM200Stand"), SIGNAL(clicked( bool )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("radDM200GS1"), SIGNAL(clicked( bool )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("radDM200HIBC"), SIGNAL(clicked( bool )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("cmbDM200Size"), SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("chkDMRectangle"), SIGNAL(stateChanged( int )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("chkDMRE"), SIGNAL(stateChanged( int )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("chkDMGSSep"), SIGNAL(stateChanged( int )), SLOT(update_preview()));
    }

    if (symbology == BARCODE_ITF14)
    {
        btype->setItemText(0, tr("Default (box, non-zero width)"));
    }

    if (symbology == BARCODE_QRCODE)
    {
        QFile file(":/grpQR.ui");
        if (!file.open(QIODevice::ReadOnly))
            return;
        m_optionWidget=uiload.load(&file);
        file.close();
        tabMain->insertTab(1,m_optionWidget,tr("QR Cod&e"));
        connect(m_optionWidget->findChild<QObject*>("cmbQRSize"), SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("cmbQRECC"), SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("radQRStand"), SIGNAL(clicked( bool )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("radQRGS1"), SIGNAL(clicked( bool )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("radQRHIBC"), SIGNAL(clicked( bool )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("chkQRFullMultibyte"), SIGNAL(stateChanged( int )), SLOT(update_preview()));
    }

    if (symbology == BARCODE_RMQR)
    {
        QFile file(":/grpRMQR.ui");
        if (!file.open(QIODevice::ReadOnly))
            return;
        m_optionWidget=uiload.load(&file);
        file.close();
        tabMain->insertTab(1,m_optionWidget,tr("rMQR Cod&e"));
        connect(m_optionWidget->findChild<QObject*>("cmbRMQRSize"), SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("cmbRMQRECC"), SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("radRMQRStand"), SIGNAL(clicked( bool )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("radRMQRGS1"), SIGNAL(clicked( bool )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("chkRMQRFullMultibyte"), SIGNAL(stateChanged( int )), SLOT(update_preview()));
    }

    if (symbology == BARCODE_HANXIN)
    {
        QFile file (":/grpHX.ui");
        if (!file.open(QIODevice::ReadOnly))
            return;
        m_optionWidget=uiload.load(&file);
        file.close();
        tabMain->insertTab(1,m_optionWidget,tr("Han Xin Cod&e"));
        connect(m_optionWidget->findChild<QObject*>("cmbHXSize"), SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("cmbHXECC"), SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("chkHXFullMultibyte"), SIGNAL(stateChanged( int )), SLOT(update_preview()));
    }

    if (symbology == BARCODE_MICROQR)
    {
        QFile file(":/grpMQR.ui");
        if (!file.open(QIODevice::ReadOnly))
            return;
        m_optionWidget=uiload.load(&file);
        file.close();
        tabMain->insertTab(1,m_optionWidget,tr("Micro QR Cod&e"));
        connect(m_optionWidget->findChild<QObject*>("cmbMQRSize"), SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("cmbMQRECC"), SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("chkMQRFullMultibyte"), SIGNAL(stateChanged( int )), SLOT(update_preview()));
    }

    if (symbology == BARCODE_GRIDMATRIX)
    {
        QFile file(":/grpGrid.ui");
        if (!file.open(QIODevice::ReadOnly))
            return;
        m_optionWidget=uiload.load(&file);
        file.close();
        tabMain->insertTab(1,m_optionWidget,tr("Grid Matrix"));
        connect(m_optionWidget->findChild<QObject*>("cmbGridSize"), SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("cmbGridECC"), SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("chkGridFullMultibyte"), SIGNAL(stateChanged( int )), SLOT(update_preview()));
    }

    if (symbology == BARCODE_MAXICODE)
    {
        QFile file(":/grpMaxicode.ui");
        if (!file.open(QIODevice::ReadOnly))
            return;
        m_optionWidget=uiload.load(&file);
        file.close();
        tabMain->insertTab(1,m_optionWidget,tr("Maxicod&e"));
        connect(m_optionWidget->findChild<QObject*>("cmbMaxiMode"), SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("cmbMaxiMode"), SIGNAL(currentIndexChanged( int )), SLOT(maxi_primary()));
        connect(m_optionWidget->findChild<QObject*>("txtMaxiPrimary"), SIGNAL(textChanged( const QString& )), SLOT(update_preview()));
    }

    if (symbology == BARCODE_CHANNEL)
    {
        QFile file(":/grpChannel.ui");
        if (!file.open(QIODevice::ReadOnly))
            return;
        m_optionWidget=uiload.load(&file);
        file.close();
        tabMain->insertTab(1,m_optionWidget,tr("Channel Cod&e"));
        connect(m_optionWidget->findChild<QObject*>("cmbChannel"), SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
    }

    if (symbology == BARCODE_CODEONE)
    {
        QFile file(":/grpCodeOne.ui");
        if (!file.open(QIODevice::ReadOnly))
            return;
        m_optionWidget=uiload.load(&file);
        file.close();
        tabMain->insertTab(1,m_optionWidget,tr("Code On&e"));
        connect(m_optionWidget->findChild<QObject*>("cmbC1Size"), SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("radC1GS1"), SIGNAL(toggled( bool )), SLOT(update_preview()));
    }

    if (symbology == BARCODE_CODE49)
    {
        QFile file(":/grpC49.ui");
        if (!file.open(QIODevice::ReadOnly))
            return;
        m_optionWidget=uiload.load(&file);
        file.close();
        tabMain->insertTab(1,m_optionWidget,tr("Cod&e 49"));
        connect(m_optionWidget->findChild<QObject*>("cmbC49RowSepHeight"), SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("radC49GS1"), SIGNAL(toggled( bool )), SLOT(update_preview()));
    }

    if (symbology == BARCODE_DBAR_EXPSTK)
    {
        QFile file(":/grpDBExtend.ui");
        if (!file.open(QIODevice::ReadOnly))
            return;
        m_optionWidget=uiload.load(&file);
        file.close();
        tabMain->insertTab(1,m_optionWidget,tr("GS1 DataBar Stack&ed"));
        connect(m_optionWidget->findChild<QObject*>("cmbCols"), SIGNAL(currentIndexChanged ( int )), SLOT(update_preview()));
    }

    if (symbology == BARCODE_ULTRA)
    {
        QFile file(":/grpUltra.ui");
        if (!file.open(QIODevice::ReadOnly))
            return;
        m_optionWidget=uiload.load(&file);
        file.close();
        tabMain->insertTab(1,m_optionWidget,tr("Ultracod&e"));
        connect(m_optionWidget->findChild<QObject*>("radUltraAuto"), SIGNAL(clicked( bool )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("radUltraEcc"), SIGNAL(clicked( bool )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("cmbUltraEcc"), SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("radUltraStand"), SIGNAL(clicked( bool )), SLOT(update_preview()));
        connect(m_optionWidget->findChild<QObject*>("radUltraGS1"), SIGNAL(clicked( bool )), SLOT(update_preview()));
    }

    if (symbology == BARCODE_UPCA || symbology == BARCODE_UPCA_CHK || symbology == BARCODE_UPCA_CC)
    {
        QFile file(":/grpUPCA.ui");
        if (!file.open(QIODevice::ReadOnly))
            return;
        m_optionWidget=uiload.load(&file);
        file.close();
        tabMain->insertTab(1, m_optionWidget, tr("UPC-A"));
        combobox_item_enabled(cmbFontSetting, 1, false);
        if (cmbFontSetting->currentIndex() == 1) {
            cmbFontSetting->setCurrentIndex(0);
        }
        cmbFontSetting->setItemText(2, tr("Small (vector only)"));
        connect(m_optionWidget->findChild<QObject*>("cmbUPCAAddonGap"), SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
    }

    if (symbology == BARCODE_EANX || symbology == BARCODE_EANX_CHK || symbology == BARCODE_EANX_CC
            || symbology == BARCODE_UPCE || symbology == BARCODE_UPCE_CHK || symbology == BARCODE_UPCE_CC
            || symbology == BARCODE_ISBNX)
    {
        QFile file(":/grpUPCEAN.ui");
        if (!file.open(QIODevice::ReadOnly))
            return;
        m_optionWidget=uiload.load(&file);
        file.close();
        if (symbology == BARCODE_UPCE || symbology == BARCODE_UPCE_CHK || symbology == BARCODE_UPCE_CC) {
            tabMain->insertTab(1, m_optionWidget, tr("UPC-&E"));
        } else if (symbology == BARCODE_ISBNX) {
            tabMain->insertTab(1, m_optionWidget, tr("ISBN"));
        } else {
            tabMain->insertTab(1, m_optionWidget, tr("&EAN"));
        }
        combobox_item_enabled(cmbFontSetting, 1, false);
        if (cmbFontSetting->currentIndex() == 1) {
            cmbFontSetting->setCurrentIndex(0);
        }
        cmbFontSetting->setItemText(2, tr("Small (vector only)"));
        connect(m_optionWidget->findChild<QObject*>("cmbUPCEANAddonGap"), SIGNAL(currentIndexChanged( int )), SLOT(update_preview()));
    }

    if (symbology == BARCODE_VIN)
    {
        QFile file(":/grpVIN.ui");
        if (!file.open(QIODevice::ReadOnly))
            return;
        m_optionWidget=uiload.load(&file);
        file.close();
        tabMain->insertTab(1,m_optionWidget,tr("&VIN"));
        connect(m_optionWidget->findChild<QObject*>("chkVINImportChar"), SIGNAL(clicked( bool )), SLOT(update_preview()));
    }

    switch (symbology)
    {
        case BARCODE_CODE128:
        case BARCODE_EANX:
        case BARCODE_UPCA:
        case BARCODE_UPCE:
        case BARCODE_DBAR_OMN:
        case BARCODE_DBAR_LTD:
        case BARCODE_DBAR_EXP:
        case BARCODE_DBAR_STK:
        case BARCODE_DBAR_OMNSTK:
        case BARCODE_DBAR_EXPSTK:
            grpComposite->show();
            break;
        default:
            grpComposite->hide();
            break;
    }
    cmbECI->setEnabled(m_bc.bc.supportsECI(symbology)); /* Will need checking again in update_preview() as encoding mode dependent (HIBC) */
    chkRInit->setEnabled(m_bc.bc.supportsReaderInit(symbology)); /* Ditto (HIBC and GS1) */
    chkAutoHeight->setEnabled(!m_bc.bc.isFixedRatio(symbology));
    chkHRTShow->setEnabled(m_bc.bc.hasHRT(symbology));
    chkDotty->setEnabled(m_bc.bc.isDotty(symbology));

    load_sub_settings(settings, symbology);

    composite_ui_set();
    autoheight_ui_set();
    HRTShow_ui_set();
    dotty_ui_set();

    if (initial_load) {
        tabMain->setCurrentIndex(settings.value("studio/tab_index", 0).toInt());
    } else if (original_tab_count == tabMain->count()) {
        tabMain->setCurrentIndex(original_tab_index);
    } else if (original_tab_count > tabMain->count()) {
        tabMain->setCurrentIndex(original_tab_index == 2 ? 1 : 0);
    } else {
        tabMain->setCurrentIndex(original_tab_index == 1 ? 2 : 0);
    }
}

void MainWindow::composite_ui_set()
{
    if (!grpComposite->isHidden() && chkComposite->isChecked())
    {
        lblCompType->setEnabled(true);
        cmbCompType->setEnabled(true);
        lblComposite->setEnabled(true);
        txtComposite->setEnabled(true);
        if (metaObject()->enumerator(0).value(bstyle->currentIndex()) == BARCODE_CODE128) {
            QRadioButton *radioButton = m_optionWidget->findChild<QRadioButton*>("radC128EAN");
            if (radioButton) {
                radioButton->setChecked(true);
            }
        }
    }
    else
    {
        lblCompType->setEnabled(false);
        cmbCompType->setEnabled(false);
        lblComposite->setEnabled(false);
        txtComposite->setEnabled(false);
    }
}

void MainWindow::composite_ean_check()
{
    if (metaObject()->enumerator(0).value(bstyle->currentIndex())!=BARCODE_CODE128)
        return;
    QRadioButton *radioButton = m_optionWidget->findChild<QRadioButton*>("radC128EAN");
    if (radioButton && !radioButton->isChecked())
        chkComposite->setChecked(false);
}

void MainWindow::maxi_primary()
{
    if (metaObject()->enumerator(0).value(bstyle->currentIndex())!=BARCODE_MAXICODE)
        return;
    if(m_optionWidget->findChild<QComboBox*>("cmbMaxiMode")->currentIndex() == 0) {
        m_optionWidget->findChild<QLabel*>("lblMaxiPrimary")->setEnabled(true);
        m_optionWidget->findChild<QLineEdit*>("txtMaxiPrimary")->setEnabled(true);
    } else {
        m_optionWidget->findChild<QLabel*>("lblMaxiPrimary")->setEnabled(false);
        m_optionWidget->findChild<QLineEdit*>("txtMaxiPrimary")->setEnabled(false);
    }
}

// Taken from https://stackoverflow.com/questions/38915001/disable-specific-items-in-qcombobox
void MainWindow::combobox_item_enabled(QComboBox *comboBox, int index, bool enabled)
{
    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(comboBox->model());
    if (model) {
        QStandardItem *item = model->item(index);
        if (item) {
            item->setEnabled(enabled);
        }
    }
}

void MainWindow::upcean_addon_gap(QComboBox *comboBox, QLabel *label, int base)
{
    const QRegularExpression addonRE("^[0-9X]+[+][0-9]+$");
    int item_val;

    if (txtData->text().contains(addonRE)) {
        comboBox->setEnabled(true);
        label->setEnabled(true);
        item_val = comboBox->currentIndex();
        if (item_val) {
            m_bc.bc.setOption2(item_val + base);
        }
    } else {
        comboBox->setEnabled(false);
        label->setEnabled(false);
    }
}

void MainWindow::set_gs1_mode(bool gs1_mode)
{
    if (gs1_mode) {
        m_bc.bc.setInputMode(GS1_MODE | (m_bc.bc.inputMode() & ~0x07)); // Keep upper bits
        chkData->setEnabled(false);
    } else {
        chkData->setEnabled(true);
    }
}

void MainWindow::update_preview()
{
    int symbology = metaObject()->enumerator(0).value(bstyle->currentIndex());
    int width = view->geometry().width();
    int height = view->geometry().height();
    int item_val;
    QCheckBox *checkBox;

    //m_bc.ar=(Zint::QZint::AspectRatioMode)1;
    if (!grpComposite->isHidden() && chkComposite->isChecked()) {
        m_bc.bc.setPrimaryMessage(txtData->text());
        m_bc.bc.setText(txtComposite->toPlainText());
    } else {
        m_bc.bc.setText(txtData->text());
    }
    m_bc.bc.setSecurityLevel(0);
    m_bc.bc.setOption2(0);
    m_bc.bc.setOption3(0);
    chkData->setEnabled(true);
    if (chkData->isChecked()) {
        m_bc.bc.setInputMode(DATA_MODE);
    } else {
        m_bc.bc.setInputMode(UNICODE_MODE);
    }
    if (chkEscape->isChecked()) {
        m_bc.bc.setInputMode(m_bc.bc.inputMode() | ESCAPE_MODE);
    }
    m_bc.bc.setGSSep(false);
    m_bc.bc.setDotSize(0.4f / 0.5f);

    switch (symbology) {

        case BARCODE_CODE128:
            if(m_optionWidget->findChild<QRadioButton*>("radC128Stand")->isChecked())
                m_bc.bc.setSymbol(BARCODE_CODE128);

            if(m_optionWidget->findChild<QRadioButton*>("radC128CSup")->isChecked())
                m_bc.bc.setSymbol(BARCODE_CODE128B);

            if(m_optionWidget->findChild<QRadioButton*>("radC128EAN")->isChecked())
            {
                if(chkComposite->isChecked())
                    m_bc.bc.setSymbol(BARCODE_GS1_128_CC);
                else
                    m_bc.bc.setSymbol(BARCODE_GS1_128);
            }

            if(m_optionWidget->findChild<QRadioButton*>("radC128HIBC")->isChecked())
                m_bc.bc.setSymbol(BARCODE_HIBC_128);
            break;

        case BARCODE_EANX:
            if(chkComposite->isChecked())
                m_bc.bc.setSymbol(BARCODE_EANX_CC);
            else
                m_bc.bc.setSymbol(BARCODE_EANX);
            upcean_addon_gap(m_optionWidget->findChild<QComboBox*>("cmbUPCEANAddonGap"), m_optionWidget->findChild<QLabel*>("lblUPCEANAddonGap"), 7 /*base*/);
            break;

        case BARCODE_ISBNX:
            m_bc.bc.setSymbol(symbology);
            upcean_addon_gap(m_optionWidget->findChild<QComboBox*>("cmbUPCEANAddonGap"), m_optionWidget->findChild<QLabel*>("lblUPCEANAddonGap"), 7 /*base*/);
            break;

        case BARCODE_UPCA:
            if(chkComposite->isChecked())
                m_bc.bc.setSymbol(BARCODE_UPCA_CC);
            else
                m_bc.bc.setSymbol(BARCODE_UPCA);
            upcean_addon_gap(m_optionWidget->findChild<QComboBox*>("cmbUPCAAddonGap"), m_optionWidget->findChild<QLabel*>("lblUPCAAddonGap"), 9 /*base*/);
            break;

        case BARCODE_UPCE:
            if(chkComposite->isChecked())
                m_bc.bc.setSymbol(BARCODE_UPCE_CC);
            else
                m_bc.bc.setSymbol(BARCODE_UPCE);
            upcean_addon_gap(m_optionWidget->findChild<QComboBox*>("cmbUPCEANAddonGap"), m_optionWidget->findChild<QLabel*>("lblUPCEANAddonGap"), 7 /*base*/);
            break;

        case BARCODE_DBAR_OMN:
            if(chkComposite->isChecked())
                m_bc.bc.setSymbol(BARCODE_DBAR_OMN_CC);
            else
                m_bc.bc.setSymbol(BARCODE_DBAR_OMN);
            break;

        case BARCODE_DBAR_LTD:
            if(chkComposite->isChecked())
                m_bc.bc.setSymbol(BARCODE_DBAR_LTD_CC);
            else
                m_bc.bc.setSymbol(BARCODE_DBAR_LTD);
            break;

        case BARCODE_DBAR_EXP:
            if(chkComposite->isChecked())
                m_bc.bc.setSymbol(BARCODE_DBAR_EXP_CC);
            else
                m_bc.bc.setSymbol(BARCODE_DBAR_EXP);
            break;

        case BARCODE_DBAR_STK:
            if(chkComposite->isChecked())
                m_bc.bc.setSymbol(BARCODE_DBAR_STK_CC);
            else
                m_bc.bc.setSymbol(BARCODE_DBAR_STK);
            break;

        case BARCODE_DBAR_OMNSTK:
            if(chkComposite->isChecked())
                m_bc.bc.setSymbol(BARCODE_DBAR_OMNSTK_CC);
            else
                m_bc.bc.setSymbol(BARCODE_DBAR_OMNSTK);
            break;

        case BARCODE_DBAR_EXPSTK:
            if(chkComposite->isChecked())
                m_bc.bc.setSymbol(BARCODE_DBAR_EXPSTK_CC);
            else
                m_bc.bc.setSymbol(BARCODE_DBAR_EXPSTK);

            if(m_optionWidget->findChild<QComboBox*>("cmbCols")->currentIndex() != 0)
                m_bc.bc.setOption2(m_optionWidget->findChild<QComboBox*>("cmbCols")->currentIndex());
            break;

        case BARCODE_PDF417:
            m_bc.bc.setOption2(m_optionWidget->findChild<QComboBox*>("cmbPDFCols")->currentIndex());
            m_bc.bc.setSecurityLevel(m_optionWidget->findChild<QComboBox*>("cmbPDFECC")->currentIndex()-1);
            if(m_optionWidget->findChild<QRadioButton*>("radPDFStand")->isChecked())
                m_bc.bc.setSymbol(BARCODE_PDF417);

            if(m_optionWidget->findChild<QRadioButton*>("radPDFTruncated")->isChecked())
                m_bc.bc.setSymbol(BARCODE_PDF417COMP);

            if(m_optionWidget->findChild<QRadioButton*>("radPDFHIBC")->isChecked())
                m_bc.bc.setSymbol(BARCODE_HIBC_PDF);
            break;

        case BARCODE_MICROPDF417:
            m_bc.bc.setOption2(m_optionWidget->findChild<QComboBox*>("cmbMPDFCols")->currentIndex());
            if(m_optionWidget->findChild<QRadioButton*>("radMPDFStand")->isChecked())
                m_bc.bc.setSymbol(BARCODE_MICROPDF417);

            if(m_optionWidget->findChild<QRadioButton*>("radMPDFHIBC")->isChecked())
                m_bc.bc.setSymbol(BARCODE_HIBC_MICPDF);
            break;

        case BARCODE_DOTCODE:
            m_bc.bc.setSymbol(BARCODE_DOTCODE);
            m_bc.bc.setOption2(m_optionWidget->findChild<QComboBox*>("cmbDotCols")->currentIndex());
            set_gs1_mode(m_optionWidget->findChild<QRadioButton*>("radDotGS1")->isChecked());
            break;

        case BARCODE_AZTEC:
            m_bc.bc.setSymbol(BARCODE_AZTEC);
            if(m_optionWidget->findChild<QRadioButton*>("radAztecSize")->isChecked())
                m_bc.bc.setOption2(m_optionWidget->findChild<QComboBox*>("cmbAztecSize")->currentIndex() + 1);

            if(m_optionWidget->findChild<QRadioButton*>("radAztecECC")->isChecked())
                m_bc.bc.setSecurityLevel(m_optionWidget->findChild<QComboBox*>("cmbAztecECC")->currentIndex() + 1);

            set_gs1_mode(m_optionWidget->findChild<QRadioButton*>("radAztecGS1")->isChecked());
            if(m_optionWidget->findChild<QRadioButton*>("radAztecHIBC")->isChecked())
                m_bc.bc.setSymbol(BARCODE_HIBC_AZTEC);
            break;

        case BARCODE_MSI_PLESSEY:
            m_bc.bc.setSymbol(BARCODE_MSI_PLESSEY);
            m_bc.bc.setOption2(m_optionWidget->findChild<QComboBox*>("cmbMSICheck")->currentIndex());
            break;

        case BARCODE_CODE11:
            m_bc.bc.setSymbol(BARCODE_CODE11);
            if (m_optionWidget->findChild<QRadioButton*>("radC11OneCheckDigit")->isChecked()) {
                m_bc.bc.setOption2(1);
            } else if (m_optionWidget->findChild<QRadioButton*>("radC11NoCheckDigits")->isChecked()) {
                m_bc.bc.setOption2(2);
            }
            break;

        case BARCODE_CODE39:
            if(m_optionWidget->findChild<QRadioButton*>("radC39HIBC")->isChecked())
                m_bc.bc.setSymbol(BARCODE_HIBC_39);
            else
            {
                m_bc.bc.setSymbol(BARCODE_CODE39);
                if(m_optionWidget->findChild<QRadioButton*>("radC39Check")->isChecked())
                    m_bc.bc.setOption2(1);
            }
            break;

        case BARCODE_EXCODE39:
            m_bc.bc.setSymbol(BARCODE_EXCODE39);
            if(m_optionWidget->findChild<QRadioButton*>("radC39Check")->isChecked())
                m_bc.bc.setOption2(1);

            break;

        case BARCODE_LOGMARS:
            m_bc.bc.setSymbol(BARCODE_LOGMARS);
            if (m_optionWidget->findChild<QRadioButton*>("radLOGMARSCheck")->isChecked()) {
                m_bc.bc.setOption2(1);
            }
            break;

        case BARCODE_CODE16K:
            m_bc.bc.setSymbol(BARCODE_CODE16K);
            set_gs1_mode(m_optionWidget->findChild<QRadioButton*>("radC16kGS1")->isChecked());
            // Row separator height selection uses option 3 in zint_symbol
            item_val = m_optionWidget->findChild<QComboBox*>("cmbC16kRowSepHeight")->currentIndex();
            if (item_val) {
                m_bc.bc.setOption3(item_val + 1); // Zero-based
            }
            break;

        case BARCODE_CODABAR:
            m_bc.bc.setSymbol(BARCODE_CODABAR);
            if (m_optionWidget->findChild<QCheckBox*>("chkCodabarCheck")->isChecked()) {
                m_bc.bc.setOption2(1);
            }
            break;

        case BARCODE_CODABLOCKF:
            if (m_optionWidget->findChild<QRadioButton*>("radCbfHIBC")->isChecked()) {
                m_bc.bc.setSymbol(BARCODE_HIBC_BLOCKF);
            } else {
                m_bc.bc.setSymbol(BARCODE_CODABLOCKF);
            }
            item_val = m_optionWidget->findChild<QComboBox*>("cmbCbfWidth")->currentIndex();
            if (item_val) {
                m_bc.bc.setOption2(item_val - 1 + 9);
            }
            // Height selection uses option 1 in zint_symbol
            item_val = m_optionWidget->findChild<QComboBox*>("cmbCbfHeight")->currentIndex();
            if (item_val) {
                m_bc.bc.setSecurityLevel(item_val);
            }
            // Row separator height selection uses option 3 in zint_symbol
            item_val = m_optionWidget->findChild<QComboBox*>("cmbCbfRowSepHeight")->currentIndex();
            if (item_val) {
                m_bc.bc.setOption3(item_val + 1); // Zero-based
            }
            break;

        case BARCODE_DATAMATRIX:
            m_bc.bc.setSecurityLevel(1);
            if(m_optionWidget->findChild<QRadioButton*>("radDM200HIBC")->isChecked())
                m_bc.bc.setSymbol(BARCODE_HIBC_DM);
            else
                m_bc.bc.setSymbol(BARCODE_DATAMATRIX);

            if (m_optionWidget->findChild<QRadioButton*>("radDM200GS1")->isChecked()) {
                set_gs1_mode(true);
                checkBox = m_optionWidget->findChild<QCheckBox*>("chkDMGSSep");
                checkBox->setEnabled(true);
                if (checkBox->isChecked()) {
                    m_bc.bc.setGSSep(true);
                }
            } else {
                set_gs1_mode(false);
                m_optionWidget->findChild<QCheckBox*>("chkDMGSSep")->setEnabled(false);
            }

            m_bc.bc.setOption2(m_optionWidget->findChild<QComboBox*>("cmbDM200Size")->currentIndex());

            if (m_optionWidget->findChild<QComboBox*>("cmbDM200Size")->currentIndex() == 0) {
                // Supressing rectangles or allowing DMRE only makes sense if in automatic size mode
                findChild<QCheckBox*>("chkDMRectangle")->setEnabled(true);
                findChild<QCheckBox*>("chkDMRE")->setEnabled(true);
                if(m_optionWidget->findChild<QCheckBox*>("chkDMRectangle")->isChecked())
                        m_bc.bc.setOption3(DM_SQUARE);
                else {
                    if(m_optionWidget->findChild<QCheckBox*>("chkDMRE")->isChecked())
                        m_bc.bc.setOption3(DM_DMRE);
                    else
                        m_bc.bc.setOption3(0);
                }
            } else {
                findChild<QCheckBox*>("chkDMRectangle")->setEnabled(false);
                findChild<QCheckBox*>("chkDMRE")->setEnabled(false);
                m_bc.bc.setOption3(0);
            }
            break;

        case BARCODE_QRCODE:
            if(m_optionWidget->findChild<QRadioButton*>("radQRHIBC")->isChecked())
                m_bc.bc.setSymbol(BARCODE_HIBC_QR);
            else
                m_bc.bc.setSymbol(BARCODE_QRCODE);

            set_gs1_mode(m_optionWidget->findChild<QRadioButton*>("radQRGS1")->isChecked());

            item_val = m_optionWidget->findChild<QComboBox*>("cmbQRSize")->currentIndex();
            if (item_val) {
                m_bc.bc.setOption2(item_val);
            }
            item_val = m_optionWidget->findChild<QComboBox*>("cmbQRECC")->currentIndex();
            if (item_val) {
                m_bc.bc.setSecurityLevel(item_val);
            }
            if (m_optionWidget->findChild<QCheckBox*>("chkQRFullMultibyte")->isChecked()) {
                m_bc.bc.setOption3(ZINT_FULL_MULTIBYTE);
            }
            break;

        case BARCODE_MICROQR:
            m_bc.bc.setSymbol(BARCODE_MICROQR);
            item_val = m_optionWidget->findChild<QComboBox*>("cmbMQRSize")->currentIndex();
            if (item_val) {
                m_bc.bc.setOption2(item_val);
            }
            item_val = m_optionWidget->findChild<QComboBox*>("cmbMQRECC")->currentIndex();
            if (item_val) {
                m_bc.bc.setSecurityLevel(item_val);
            }
            if (m_optionWidget->findChild<QCheckBox*>("chkMQRFullMultibyte")->isChecked()) {
                m_bc.bc.setOption3(ZINT_FULL_MULTIBYTE);
            }
            break;

        case BARCODE_RMQR:
            m_bc.bc.setSymbol(BARCODE_RMQR);

            set_gs1_mode(m_optionWidget->findChild<QRadioButton*>("radRMQRGS1")->isChecked());

            item_val = m_optionWidget->findChild<QComboBox*>("cmbRMQRSize")->currentIndex();
            if (item_val) {
                m_bc.bc.setOption2(item_val);
            }
            item_val = m_optionWidget->findChild<QComboBox*>("cmbRMQRECC")->currentIndex();
            if (item_val) {
                m_bc.bc.setSecurityLevel(item_val * 2); // Levels 2 (M) and 4 (H) only
            }
            if (m_optionWidget->findChild<QCheckBox*>("chkRMQRFullMultibyte")->isChecked()) {
                m_bc.bc.setOption3(ZINT_FULL_MULTIBYTE);
            }
            break;

        case BARCODE_GRIDMATRIX:
            m_bc.bc.setSymbol(BARCODE_GRIDMATRIX);
            item_val = m_optionWidget->findChild<QComboBox*>("cmbGridSize")->currentIndex();
            if (item_val) {
                m_bc.bc.setOption2(item_val);
            }
            item_val = m_optionWidget->findChild<QComboBox*>("cmbGridECC")->currentIndex();
            if (item_val) {
                m_bc.bc.setSecurityLevel(item_val);
            }
            if (m_optionWidget->findChild<QCheckBox*>("chkGridFullMultibyte")->isChecked()) {
                m_bc.bc.setOption3(ZINT_FULL_MULTIBYTE);
            }
            break;

        case BARCODE_MAXICODE:
            m_bc.bc.setSymbol(BARCODE_MAXICODE);
            if(m_optionWidget->findChild<QComboBox*>("cmbMaxiMode")->currentIndex() == 0)
            {
                m_bc.bc.setSecurityLevel(2);
                m_bc.bc.setPrimaryMessage(m_optionWidget->findChild<QLineEdit*>("txtMaxiPrimary")->text());
            }
            else
                m_bc.bc.setSecurityLevel(m_optionWidget->findChild<QComboBox*>("cmbMaxiMode")->currentIndex() + 3);
            break;

        case BARCODE_CHANNEL:
            m_bc.bc.setSymbol(BARCODE_CHANNEL);
            if(m_optionWidget->findChild<QComboBox*>("cmbChannel")->currentIndex() == 0)
                m_bc.bc.setOption2(0);
            else
                m_bc.bc.setOption2(m_optionWidget->findChild<QComboBox*>("cmbChannel")->currentIndex() + 2);
            break;

        case BARCODE_CODEONE:
            m_bc.bc.setSymbol(BARCODE_CODEONE);
            set_gs1_mode(m_optionWidget->findChild<QRadioButton*>("radC1GS1")->isChecked());
            m_bc.bc.setOption2(m_optionWidget->findChild<QComboBox*>("cmbC1Size")->currentIndex());
            break;

        case BARCODE_CODE49:
            m_bc.bc.setSymbol(BARCODE_CODE49);
            set_gs1_mode(m_optionWidget->findChild<QRadioButton*>("radC49GS1")->isChecked());
            // Row separator height selection uses option 3 in zint_symbol
            item_val = m_optionWidget->findChild<QComboBox*>("cmbC49RowSepHeight")->currentIndex();
            if (item_val) {
                m_bc.bc.setOption3(item_val + 1); // Zero-based
            }
            break;

        case BARCODE_HANXIN:
            m_bc.bc.setSymbol(BARCODE_HANXIN);
            item_val = m_optionWidget->findChild<QComboBox*>("cmbHXSize")->currentIndex();
            if (item_val) {
                m_bc.bc.setOption2(item_val);
            }
            item_val = m_optionWidget->findChild<QComboBox*>("cmbHXECC")->currentIndex();
            if (item_val) {
                m_bc.bc.setSecurityLevel(item_val);
            }
            if (m_optionWidget->findChild<QCheckBox*>("chkHXFullMultibyte")->isChecked()) {
                m_bc.bc.setOption3(ZINT_FULL_MULTIBYTE);
            }
            break;

        case BARCODE_ULTRA:
            m_bc.bc.setSymbol(BARCODE_ULTRA);
            if(m_optionWidget->findChild<QRadioButton*>("radUltraEcc")->isChecked())
                m_bc.bc.setSecurityLevel(m_optionWidget->findChild<QComboBox*>("cmbUltraEcc")->currentIndex() + 1);
            set_gs1_mode(m_optionWidget->findChild<QRadioButton*>("radUltraGS1")->isChecked());
            break;

        case BARCODE_VIN:
            m_bc.bc.setSymbol(BARCODE_VIN);
            if (m_optionWidget->findChild<QCheckBox*>("chkVINImportChar")->isChecked()) {
                m_bc.bc.setOption2(m_bc.bc.option2() | 1); // Import character 'I' prefix
            }
            break;

        default:
            m_bc.bc.setSymbol(symbology);
            break;
    }
    m_symbology = m_bc.bc.symbol();

    /* Recheck ECI and Reader Init */
    cmbECI->setEnabled(m_bc.bc.supportsECI());
    lblECI->setEnabled(cmbECI->isEnabled());
    chkRInit->setEnabled(m_bc.bc.supportsReaderInit() && (m_bc.bc.inputMode() & 0x07) != GS1_MODE);

    if (!grpComposite->isHidden() && chkComposite->isChecked())
        m_bc.bc.setSecurityLevel(cmbCompType->currentIndex());

    if (!chkAutoHeight->isEnabled() || chkAutoHeight->isChecked()) {
        m_bc.bc.setHeight(0);
    } else {
        m_bc.bc.setHeight(heightb->value());
    }
    m_bc.bc.setECI(cmbECI->isEnabled() ? cmbECI->currentIndex() : 0);
    m_bc.bc.setReaderInit(chkRInit->isEnabled() && chkRInit->isChecked());
    m_bc.bc.setShowText(chkHRTShow->isEnabled() && chkHRTShow->isChecked());
    m_bc.bc.setBorderType(btype->currentIndex());
    m_bc.bc.setBorderWidth(bwidth->value());
    m_bc.bc.setWhitespace(spnWhitespace->value());
    m_bc.bc.setFontSetting(cmbFontSetting->currentIndex());
    m_bc.bc.setRotateAngle(cmbRotate->currentIndex());
    m_bc.bc.setDotty(chkDotty->isEnabled() && chkDotty->isChecked());
    if (m_symbology == BARCODE_DOTCODE || (chkDotty->isEnabled() && chkDotty->isChecked())) {
        m_bc.bc.setDotSize(spnDotSize->value());
    }
    m_bc.bc.setFgColor(m_fgcolor);
    m_bc.bc.setBgColor(m_bgcolor);
    change_print_scale();
    change_cmyk();
    m_bc.setSize(width - 10, height - 10);
    m_bc.update();
    scene->setSceneRect(m_bc.boundingRect());
    scene->update();
}

/* Return settings subsection name for a symbol */
const char *MainWindow::get_setting_name(int symbology) {
    struct item {
        const char *name;
        int define;
        int val;
    };
    static const struct item data[] = {
        { "", -1, 0 },
        { "code11", BARCODE_CODE11, 1 },
        { "c25standard", BARCODE_C25STANDARD, 2 },
        { "c25inter", BARCODE_C25INTER, 3 },
        { "c25iata", BARCODE_C25IATA, 4 },
        { "", -1, 5 },
        { "c25logic", BARCODE_C25LOGIC, 6 },
        { "c25ind", BARCODE_C25IND, 7 },
        { "code39", BARCODE_CODE39, 8 },
        { "excode39", BARCODE_EXCODE39, 9 },
        { "", -1, 10 },
        { "", -1, 11 },
        { "", -1, 12 },
        { "eanx", BARCODE_EANX, 13 },
        { "eanx", BARCODE_EANX_CHK, 14 },
        { "", -1, 15 },
        { "code128", BARCODE_GS1_128, 16 },
        { "", -1, 17 },
        { "codabar", BARCODE_CODABAR, 18 },
        { "", -1, 19 },
        { "code128", BARCODE_CODE128, 20 },
        { "dpleit", BARCODE_DPLEIT, 21 },
        { "dpident", BARCODE_DPIDENT, 22 },
        { "code16k", BARCODE_CODE16K, 23 },
        { "code49", BARCODE_CODE49, 24 },
        { "code93", BARCODE_CODE93, 25 },
        { "", -1, 26 },
        { "", -1, 27 },
        { "flat", BARCODE_FLAT, 28 },
        { "dbar_omn", BARCODE_DBAR_OMN, 29 },
        { "dbar_ltd", BARCODE_DBAR_LTD, 30 },
        { "dbar_exp", BARCODE_DBAR_EXP, 31 },
        { "telepen", BARCODE_TELEPEN, 32 },
        { "", -1, 33 },
        { "upca", BARCODE_UPCA, 34 },
        { "upca", BARCODE_UPCA_CHK, 35 },
        { "", -1, 36 },
        { "upce", BARCODE_UPCE, 37 },
        { "upce", BARCODE_UPCE_CHK, 38 },
        { "", -1, 39 },
        { "postnet", BARCODE_POSTNET, 40 },
        { "", -1, 41 },
        { "", -1, 42 },
        { "", -1, 43 },
        { "", -1, 44 },
        { "", -1, 45 },
        { "", -1, 46 },
        { "msi_plessey", BARCODE_MSI_PLESSEY, 47 },
        { "", -1, 48 },
        { "fim", BARCODE_FIM, 49 },
        { "logmars", BARCODE_LOGMARS, 50 },
        { "pharma", BARCODE_PHARMA, 51 },
        { "pzn", BARCODE_PZN, 52 },
        { "pharma_two", BARCODE_PHARMA_TWO, 53 },
        { "", -1, 54 },
        { "pdf417", BARCODE_PDF417, 55 },
        { "pdf417", BARCODE_PDF417COMP, 56 },
        { "maxicode", BARCODE_MAXICODE, 57 },
        { "qrcode", BARCODE_QRCODE, 58 },
        { "", -1, 59 },
        { "code128", BARCODE_CODE128B, 60 },
        { "", -1, 61 },
        { "", -1, 62 },
        { "auspost", BARCODE_AUSPOST, 63 },
        { "", -1, 64 },
        { "", -1, 65 },
        { "ausreply", BARCODE_AUSREPLY, 66 },
        { "ausroute", BARCODE_AUSROUTE, 67 },
        { "ausredirect", BARCODE_AUSREDIRECT, 68 },
        { "isbnx", BARCODE_ISBNX, 69 },
        { "rm4scc", BARCODE_RM4SCC, 70 },
        { "datamatrix", BARCODE_DATAMATRIX, 71 },
        { "ean14", BARCODE_EAN14, 72 },
        { "vin", BARCODE_VIN, 73 },
        { "codablockf", BARCODE_CODABLOCKF, 74 },
        { "nve18", BARCODE_NVE18, 75 },
        { "japanpost", BARCODE_JAPANPOST, 76 },
        { "koreapost", BARCODE_KOREAPOST, 77 },
        { "", -1, 78 },
        { "dbar_stk", BARCODE_DBAR_STK, 79 },
        { "dbar_omnstk", BARCODE_DBAR_OMNSTK, 80 },
        { "dbar_expstk", BARCODE_DBAR_EXPSTK, 81 },
        { "planet", BARCODE_PLANET, 82 },
        { "", -1, 83 },
        { "micropdf417", BARCODE_MICROPDF417, 84 },
        { "usps_imail", BARCODE_USPS_IMAIL, 85 },
        { "plessey", BARCODE_PLESSEY, 86 },
        { "telepen_num", BARCODE_TELEPEN_NUM, 87 },
        { "", -1, 88 },
        { "itf14", BARCODE_ITF14, 89 },
        { "kix", BARCODE_KIX, 90 },
        { "", -1, 91 },
        { "aztec", BARCODE_AZTEC, 92 },
        { "daft", BARCODE_DAFT, 93 },
        { "", -1, 94 },
        { "", -1, 95 },
        { "dpd", BARCODE_DPD, 96 },
        { "microqr", BARCODE_MICROQR, 97 },
        { "code128", BARCODE_HIBC_128, 98 },
        { "code39", BARCODE_HIBC_39, 99 },
        { "", -1, 100 },
        { "", -1, 101 },
        { "datamatrix", BARCODE_HIBC_DM, 102 },
        { "", -1, 103 },
        { "qrcode", BARCODE_HIBC_QR, 104 },
        { "", -1, 105 },
        { "pdf417", BARCODE_HIBC_PDF, 106 },
        { "", -1, 107 },
        { "micropdf417", BARCODE_HIBC_MICPDF, 108 },
        { "", -1, 109 },
        { "codablockf", BARCODE_HIBC_BLOCKF, 110 },
        { "", -1, 111 },
        { "aztec", BARCODE_HIBC_AZTEC, 112 },
        { "", -1, 113 },
        { "", -1, 114 },
        { "dotcode", BARCODE_DOTCODE, 115 },
        { "hanxin", BARCODE_HANXIN, 116 },
        { "", -1, 117 },
        { "", -1, 118 },
        { "", -1, 119 },
        { "", -1, 120 },
        { "mailmark", BARCODE_MAILMARK, 121 },
        { "", -1, 122 },
        { "", -1, 123 },
        { "", -1, 124 },
        { "", -1, 125 },
        { "", -1, 126 },
        { "", -1, 127 },
        { "azrune", BARCODE_AZRUNE, 128 },
        { "code32", BARCODE_CODE32, 129 },
        { "eanx", BARCODE_EANX_CC, 130 },
        { "code128", BARCODE_GS1_128_CC, 131 },
        { "dbar_omn", BARCODE_DBAR_OMN_CC, 132 },
        { "dbar_ltd", BARCODE_DBAR_LTD_CC, 133 },
        { "dbar_exp", BARCODE_DBAR_EXP_CC, 134 },
        { "upca", BARCODE_UPCA_CC, 135 },
        { "upce", BARCODE_UPCE_CC, 136 },
        { "dbar_stk", BARCODE_DBAR_STK_CC, 137 },
        { "dbar_omnstk", BARCODE_DBAR_OMNSTK_CC, 138 },
        { "dbar_expstk", BARCODE_DBAR_EXPSTK_CC, 139 },
        { "channel", BARCODE_CHANNEL, 140 },
        { "codeone", BARCODE_CODEONE, 141 },
        { "gridmatrix", BARCODE_GRIDMATRIX, 142 },
        { "upnqr", BARCODE_UPNQR, 143 },
        { "ultra", BARCODE_ULTRA, 144 },
        { "rmqr", BARCODE_RMQR, 145 },
    };
    static const int data_size = sizeof(data) / sizeof(struct item);

    if (symbology < 0 || symbology >= data_size) {
        return "";
    }
    if (data[symbology].val != symbology || (data[symbology].define != -1 && data[symbology].define != symbology)) { // Self-check
        fprintf(stderr, "MainWindow::get_setting_name: data table out of sync (%d)\n", symbology);
        return "";
    }
    return data[symbology].name;
}

/* Helper to return index of selected radio button in group, checking for NULL */
int MainWindow::get_button_group_index(const QStringList &children) {
    QRadioButton *radioButton;
    for (int index = 0; index < children.size(); index++) {
        radioButton = m_optionWidget->findChild<QRadioButton*>(children[index]);
        if (radioButton && radioButton->isChecked()) {
            return index;
        }
    }
    return 0;
}

/* Helper to set radio button in group from index in settings, checking for NULL */
void MainWindow::set_radiobutton_from_setting(QSettings &settings, const QString &setting, const QStringList &children, int default_val) {
    int index = settings.value(setting, default_val).toInt();
    QRadioButton *radioButton;
    if (index >= 0 && index < children.size()) {
        radioButton = m_optionWidget->findChild<QRadioButton*>(children[index]);
    } else {
        radioButton = m_optionWidget->findChild<QRadioButton*>(children[0]);
    }
    if (radioButton) {
        radioButton->setChecked(true);
    }
}

/* Helper to return index of selected item in combobox, checking for NULL */
int MainWindow::get_combobox_index(const QString &child) {
    QComboBox *comboBox = m_optionWidget->findChild<QComboBox*>(child);
    return comboBox ? comboBox->currentIndex() : 0;
}

/* Helper to set item in combobox from index in settings, checking for NULL */
void MainWindow::set_combobox_from_setting(QSettings &settings, const QString &setting, const QString &child, int default_val) {
    QComboBox *comboBox = m_optionWidget->findChild<QComboBox*>(child);
    if (comboBox) {
        comboBox->setCurrentIndex(settings.value(setting, default_val).toInt());
    }
}

/* Helper to return if checkbox checked, checking for NULL */
int MainWindow::get_checkbox_val(const QString &child) {
    QCheckBox *checkBox = m_optionWidget->findChild<QCheckBox*>(child);
    return checkBox && checkBox->isChecked() ? 1 : 0;
}

/* Helper to set checkbox from settings, checking for NULL */
void MainWindow::set_checkbox_from_setting(QSettings &settings, const QString &setting, const QString &child, int default_val) {
    QCheckBox *checkBox = m_optionWidget->findChild<QCheckBox*>(child);
    if (checkBox) {
        checkBox->setChecked(settings.value(setting, default_val).toInt() ? true : false);
    }
}

/* Helper to return text of line edit, checking for NULL */
QString MainWindow::get_lineedit_val(const QString &child) {
    QLineEdit *lineEdit = m_optionWidget->findChild<QLineEdit*>(child);
    return lineEdit ? lineEdit->text() : "";
}

/* Helper to set line edit from settings, checking for NULL */
void MainWindow::set_lineedit_from_setting(QSettings &settings, const QString &setting, const QString &child, const char *default_val) {
    QLineEdit *lineEdit = m_optionWidget->findChild<QLineEdit*>(child);
    if (lineEdit) {
        lineEdit->setText(settings.value(setting, default_val).toString());
    }
}

/* Save settings for an individual symbol */
void MainWindow::save_sub_settings(QSettings &settings, int symbology) {

    const char *name = get_setting_name(symbology);
    if (*name) {
        settings.setValue(QString("studio/bc/%1/data").arg(name), txtData->text());
        if (!grpComposite->isHidden()) {
            settings.setValue(QString("studio/bc/%1/composite_text").arg(name), txtComposite->toPlainText());
            settings.setValue(QString("studio/bc/%1/chk_composite").arg(name), chkComposite->isChecked() ? 1 : 0);
            settings.setValue(QString("studio/bc/%1/comp_type").arg(name), cmbCompType->currentIndex());
        }
        if (cmbECI->isEnabled()) {
            settings.setValue(QString("studio/bc/%1/eci").arg(name), cmbECI->currentIndex());
        }
        settings.setValue(QString("studio/bc/%1/chk_escape").arg(name), chkEscape->isChecked() ? 1 : 0);
        settings.setValue(QString("studio/bc/%1/chk_data").arg(name), chkData->isChecked() ? 1 : 0);
        if (chkRInit->isEnabled()) {
            settings.setValue(QString("studio/bc/%1/chk_rinit").arg(name), chkRInit->isChecked() ? 1 : 0);
        }
        if (chkAutoHeight->isEnabled()) {
            settings.setValue(QString("studio/bc/%1/appearance/autoheight").arg(name), chkAutoHeight->isChecked() ? 1 : 0);
            settings.setValue(QString("studio/bc/%1/appearance/height").arg(name), heightb->value());
        }
        settings.setValue(QString("studio/bc/%1/appearance/border").arg(name), bwidth->value());
        settings.setValue(QString("studio/bc/%1/appearance/whitespace").arg(name), spnWhitespace->value());
        settings.setValue(QString("studio/bc/%1/appearance/scale").arg(name), spnScale->value());
        settings.setValue(QString("studio/bc/%1/appearance/border_type").arg(name), btype->currentIndex());
        if (chkHRTShow->isEnabled()) {
            settings.setValue(QString("studio/bc/%1/appearance/font_setting").arg(name), cmbFontSetting->currentIndex());
            settings.setValue(QString("studio/bc/%1/appearance/chk_hrt_show").arg(name), chkHRTShow->isChecked() ? 1 : 0);
        }
        settings.setValue(QString("studio/bc/%1/appearance/cmyk").arg(name), chkCMYK->isChecked() ? 1 : 0);
        settings.setValue(QString("studio/bc/%1/appearance/rotate").arg(name), cmbRotate->currentIndex());
        if (symbology == BARCODE_DOTCODE || chkDotty->isEnabled()) {
            settings.setValue(QString("studio/bc/%1/appearance/chk_dotty").arg(name), chkDotty->isChecked() ? 1 : 0);
            settings.setValue(QString("studio/bc/%1/appearance/dot_size").arg(name), spnDotSize->value());
        }
        settings.setValue(QString("studio/bc/%1/ink/red").arg(name), m_fgcolor.red());
        settings.setValue(QString("studio/bc/%1/ink/green").arg(name), m_fgcolor.green());
        settings.setValue(QString("studio/bc/%1/ink/blue").arg(name), m_fgcolor.blue());
        settings.setValue(QString("studio/bc/%1/ink/alpha").arg(name), m_fgcolor.alpha());
        settings.setValue(QString("studio/bc/%1/paper/red").arg(name), m_bgcolor.red());
        settings.setValue(QString("studio/bc/%1/paper/green").arg(name), m_bgcolor.green());
        settings.setValue(QString("studio/bc/%1/paper/blue").arg(name), m_bgcolor.blue());
        settings.setValue(QString("studio/bc/%1/paper/alpha").arg(name), m_bgcolor.alpha());
    }

    switch (symbology) {
        case BARCODE_CODE128:
        case BARCODE_CODE128B:
        case BARCODE_GS1_128:
        case BARCODE_GS1_128_CC:
        case BARCODE_HIBC_128:
            settings.setValue("studio/bc/code128/encoding_mode", get_button_group_index(QStringList() << "radC128Stand" << "radC128EAN" << "radC128CSup" << "radC128HIBC"));
            break;

        case BARCODE_PDF417:
        case BARCODE_PDF417COMP:
        case BARCODE_HIBC_PDF:
            settings.setValue("studio/bc/pdf417/cols", get_combobox_index("cmbPDFCols"));
            settings.setValue("studio/bc/pdf417/ecc", get_combobox_index("cmbPDFECC"));
            settings.setValue("studio/bc/pdf417/encoding_mode", get_button_group_index(QStringList() << "radPDFStand" << "radPDFTruncated" << "radPDFHIBC"));
            break;

        case BARCODE_MICROPDF417:
        case BARCODE_HIBC_MICPDF:
            settings.setValue("studio/bc/micropdf417/cols", get_combobox_index("cmbMPDFCols"));
            settings.setValue("studio/bc/micropdf417/encoding_mode", get_button_group_index(QStringList() << "radMPDFStand" << "radMPDFHIBC"));
            break;

        case BARCODE_DOTCODE:
            settings.setValue("studio/bc/dotcode/cols", get_combobox_index("cmbDotCols"));
            settings.setValue("studio/bc/dotcode/encoding_mode", get_button_group_index(QStringList() << "radDotStand" << "radDotGS1"));
            break;

        case BARCODE_AZTEC:
        case BARCODE_HIBC_AZTEC:
            settings.setValue("studio/bc/aztec/autoresizing", get_button_group_index(QStringList() << "radAztecAuto" << "radAztecSize" << "radAztecECC"));
            settings.setValue("studio/bc/aztec/size", get_combobox_index("cmbAztecSize"));
            settings.setValue("studio/bc/aztec/ecc", get_combobox_index("cmbAztecECC"));
            settings.setValue("studio/bc/aztec/encoding_mode", get_button_group_index(QStringList() << "radAztecStand" << "radAztecGS1" << "radAztecHIBC"));
            break;

        case BARCODE_MSI_PLESSEY:
            settings.setValue("studio/bc/msi_plessey/check_digit", get_combobox_index("cmbMSICheck"));
            break;

        case BARCODE_CODE11:
            settings.setValue("studio/bc/code11/check_digit", get_button_group_index(QStringList() << "radC11TwoCheckDigits" << "radC11OneCheckDigit" << "radC11NoCheckDigits"));
            break;

        case BARCODE_CODE39:
        case BARCODE_HIBC_39:
            settings.setValue("studio/bc/code39/check_digit", get_button_group_index(QStringList() << "radC39Stand" << "radC39Check" << "radC39HIBC"));
            break;

        case BARCODE_EXCODE39:
            settings.setValue("studio/bc/excode39/check_digit", get_button_group_index(QStringList() << "radC39Stand" << "radC39Check"));
            break;

        case BARCODE_LOGMARS:
            settings.setValue("studio/bc/logmars/check_digit", get_button_group_index(QStringList() << "radLOGMARSStand" << "radLOGMARSCheck"));
            break;

        case BARCODE_CODE16K:
            settings.setValue("studio/bc/code16k/row_sep_height", get_combobox_index("cmbC16kRowSepHeight"));
            settings.setValue("studio/bc/code16k/encoding_mode", get_button_group_index(QStringList() << "radC16kStand" << "radC16kGS1"));
            break;

        case BARCODE_CODABAR:
            settings.setValue("studio/bc/codabar/chk_check_char", get_checkbox_val("chkCodabarCheck"));
            break;

        case BARCODE_CODABLOCKF:
        case BARCODE_HIBC_BLOCKF:
            settings.setValue("studio/bc/codablockf/width", get_combobox_index("cmbCbfWidth"));
            settings.setValue("studio/bc/codablockf/height", get_combobox_index("cmbCbfHeight"));
            settings.setValue("studio/bc/codablockf/row_sep_height", get_combobox_index("cmbCbfRowSepHeight"));
            settings.setValue("studio/bc/codablockf/encoding_mode", get_button_group_index(QStringList() << "radCbfStand" << "radCbfHIBC"));
            break;

        case BARCODE_DATAMATRIX:
        case BARCODE_HIBC_DM:
            settings.setValue("studio/bc/datamatrix/size", get_combobox_index("cmbDM200Size"));
            settings.setValue("studio/bc/datamatrix/encoding_mode", get_button_group_index(QStringList() << "radDM200Stand" << "radDM200GS1" << "radDM200HIBC"));
            settings.setValue("studio/bc/datamatrix/chk_suppress_rect", get_checkbox_val("chkDMRectangle"));
            settings.setValue("studio/bc/datamatrix/chk_allow_dmre", get_checkbox_val("chkDMRE"));
            settings.setValue("studio/bc/datamatrix/chk_gs_sep", get_checkbox_val("chkDMGSSep"));
            break;

        case BARCODE_QRCODE:
        case BARCODE_HIBC_QR:
            settings.setValue("studio/bc/qrcode/size", get_combobox_index("cmbQRSize"));
            settings.setValue("studio/bc/qrcode/ecc", get_combobox_index("cmbQRECC"));
            settings.setValue("studio/bc/qrcode/encoding_mode", get_button_group_index(QStringList() << "radDM200Stand" << "radQRGS1" << "radQRHIBC"));
            settings.setValue("studio/bc/qrcode/chk_full_multibyte", get_checkbox_val("chkQRFullMultibyte"));
            break;

        case BARCODE_RMQR:
            settings.setValue("studio/bc/rmqr/size", get_combobox_index("cmbRMQRSize"));
            settings.setValue("studio/bc/rmqr/ecc", get_combobox_index("cmbRMQRECC"));
            settings.setValue("studio/bc/rmqr/encoding_mode", get_button_group_index(QStringList() << "radQRStand" << "radRMQRGS1"));
            settings.setValue("studio/bc/rmqr/chk_full_multibyte", get_checkbox_val("chkRMQRFullMultibyte"));
            break;

        case BARCODE_HANXIN:
            settings.setValue("studio/bc/hanxin/size", get_combobox_index("cmbHXSize"));
            settings.setValue("studio/bc/hanxin/ecc", get_combobox_index("cmbHXECC"));
            settings.setValue("studio/bc/hanxin/chk_full_multibyte", get_checkbox_val("chkHXFullMultibyte"));
            break;

        case BARCODE_MICROQR:
            settings.setValue("studio/bc/microqr/size", get_combobox_index("cmbMQRSize"));
            settings.setValue("studio/bc/microqr/ecc", get_combobox_index("cmbMQRECC"));
            settings.setValue("studio/bc/microqr/chk_full_multibyte", get_checkbox_val("chkMQRFullMultibyte"));
            break;

        case BARCODE_GRIDMATRIX:
            settings.setValue("studio/bc/gridmatrix/size", get_combobox_index("cmbGridSize"));
            settings.setValue("studio/bc/gridmatrix/ecc", get_combobox_index("cmbGridECC"));
            settings.setValue("studio/bc/gridmatrix/chk_full_multibyte", get_checkbox_val("chkGridFullMultibyte"));
            break;

        case BARCODE_MAXICODE:
            settings.setValue("studio/bc/maxicode/mode", get_combobox_index("cmbMaxiMode"));
            settings.setValue("studio/bc/maxicode/primary_message", get_lineedit_val("txtMaxiPrimary"));
            break;

        case BARCODE_CODEONE:
            settings.setValue("studio/bc/codeone/size", get_combobox_index("cmbC1Size"));
            settings.setValue("studio/bc/codeone/encoding_mode", get_button_group_index(QStringList() << "radC1Stand" << "radC1GS1"));
            break;

        case BARCODE_CODE49:
            settings.setValue("studio/bc/code49/row_sep_height", get_combobox_index("cmbC49RowSepHeight"));
            settings.setValue("studio/bc/code49/encoding_mode", get_button_group_index(QStringList() << "radC49Stand" << "radC49GS1"));
            break;

        case BARCODE_DBAR_EXPSTK:
        case BARCODE_DBAR_EXPSTK_CC:
            settings.setValue("studio/bc/dbar_expstk/cols", get_combobox_index("cmbCols"));
            break;

        case BARCODE_ULTRA:
            settings.setValue("studio/bc/ultra/autoresizing", get_button_group_index(QStringList() << "radUltraAuto" << "radUltraEcc"));
            settings.setValue("studio/bc/ultra/ecc", get_combobox_index("cmbUltraEcc"));
            settings.setValue("studio/bc/ultra/encoding_mode", get_button_group_index(QStringList() << "radUltraStand" << "radUltraGS1"));
            break;

        case BARCODE_UPCA:
        case BARCODE_UPCA_CHK:
        case BARCODE_UPCA_CC:
            settings.setValue("studio/bc/upca/addongap", get_combobox_index("cmbUPCAAddonGap"));
            break;

        case BARCODE_EANX:
        case BARCODE_EANX_CHK:
        case BARCODE_EANX_CC:
            settings.setValue("studio/bc/eanx/addongap", get_combobox_index("cmbUPCEANAddonGap"));
            break;

        case BARCODE_UPCE:
        case BARCODE_UPCE_CHK:
        case BARCODE_UPCE_CC:
            settings.setValue("studio/bc/upce/addongap", get_combobox_index("cmbUPCEANAddonGap"));
            break;

        case BARCODE_ISBNX:
            settings.setValue("studio/bc/isnbx/addongap", get_combobox_index("cmbUPCEANAddonGap"));
            break;

        case BARCODE_VIN:
            settings.setValue("studio/bc/vin/chk_import_char_prefix", get_checkbox_val("chkVINImportChar"));
            break;
    }
}

/* Load settings for an individual symbol */
void MainWindow::load_sub_settings(QSettings &settings, int symbology) {

    const char *name = get_setting_name(symbology);
    if (*name) {
        const QString &data = settings.value(QString("studio/bc/%1/data").arg(name), "").toString();
        if (!data.isEmpty()) {
            txtData->setText(data);
        }
        if (!grpComposite->isHidden()) {
            const QString &composite_text = settings.value(QString("studio/bc/%1/composite_text").arg(name), "").toString();
            if (!composite_text.isEmpty()) {
                txtComposite->setText(composite_text);
            }
            chkComposite->setChecked(settings.value(QString("studio/bc/%1/chk_composite").arg(name), 0).toInt() ? true : false);
            cmbCompType->setCurrentIndex(settings.value(QString("studio/bc/%1/comp_type").arg(name), 0).toInt());
        }
        if (cmbECI->isEnabled()) {
            cmbECI->setCurrentIndex(settings.value(QString("studio/bc/%1/eci").arg(name), 0).toInt());
        }
        chkEscape->setChecked(settings.value(QString("studio/bc/%1/chk_escape").arg(name)).toInt() ? true : false);
        chkData->setChecked(settings.value(QString("studio/bc/%1/chk_data").arg(name)).toInt() ? true : false);
        if (chkRInit->isEnabled()) {
            chkRInit->setChecked(settings.value(QString("studio/bc/%1/chk_rinit").arg(name)).toInt() ? true : false);
        }
        if (chkAutoHeight->isEnabled()) {
            chkAutoHeight->setChecked(settings.value(QString("studio/bc/%1/appearance/autoheight").arg(name), 1).toInt() ? true : false);
            heightb->setValue(settings.value(QString("studio/bc/%1/appearance/height").arg(name), 50).toInt());
        }
        bwidth->setValue(settings.value(QString("studio/bc/%1/appearance/border").arg(name), 0).toInt());
        spnWhitespace->setValue(settings.value(QString("studio/bc/%1/appearance/whitespace").arg(name), 0).toInt());
        spnScale->setValue(settings.value(QString("studio/bc/%1/appearance/scale").arg(name), 1.0).toFloat());
        btype->setCurrentIndex(settings.value(QString("studio/bc/%1/appearance/border_type").arg(name), 0).toInt());
        if (chkHRTShow->isEnabled()) {
            cmbFontSetting->setCurrentIndex(settings.value(QString("studio/bc/%1/appearance/font_setting").arg(name), 0).toInt());
            chkHRTShow->setChecked(settings.value(QString("studio/bc/%1/appearance/chk_hrt_show").arg(name), 1).toInt() ? true : false);
        }
        chkCMYK->setChecked(settings.value(QString("studio/bc/%1/appearance/cmyk").arg(name), 0).toInt() ? true : false);
        cmbRotate->setCurrentIndex(settings.value(QString("studio/bc/%1/appearance/rotate").arg(name), 0).toInt());
        if (symbology == BARCODE_DOTCODE || chkDotty->isEnabled()) {
            chkDotty->setChecked(settings.value(QString("studio/bc/%1/appearance/chk_dotty").arg(name), 0).toInt() ? true : false);
            spnDotSize->setValue(settings.value(QString("studio/bc/%1/appearance/dot_size").arg(name), 0.4f / 0.5f).toFloat());
        }
        m_fgcolor.setRgb(settings.value(QString("studio/bc/%1/ink/red").arg(name), 0).toInt(),
                        settings.value(QString("studio/bc/%1/ink/green").arg(name), 0).toInt(),
                        settings.value(QString("studio/bc/%1/ink/blue").arg(name), 0).toInt(),
                        settings.value(QString("studio/bc/%1/ink/alpha").arg(name), 0xff).toInt());
        m_bgcolor.setRgb(settings.value(QString("studio/bc/%1/paper/red").arg(name), 0xff).toInt(),
                        settings.value(QString("studio/bc/%1/paper/green").arg(name), 0xff).toInt(),
                        settings.value(QString("studio/bc/%1/paper/blue").arg(name), 0xff).toInt(),
                        settings.value(QString("studio/bc/%1/paper/alpha").arg(name), 0xff).toInt());
    }

    switch (symbology) {
        case BARCODE_CODE128:
        case BARCODE_CODE128B:
        case BARCODE_GS1_128:
        case BARCODE_GS1_128_CC:
        case BARCODE_HIBC_128:
            set_radiobutton_from_setting(settings, "studio/bc/code128/encoding_mode", QStringList() << "radC128Stand" << "radC128EAN" << "radC128CSup" << "radC128HIBC");
            break;

        case BARCODE_PDF417:
        case BARCODE_PDF417COMP:
        case BARCODE_HIBC_PDF:
            set_combobox_from_setting(settings, "studio/bc/pdf417/cols", "cmbPDFCols");
            set_combobox_from_setting(settings, "studio/bc/pdf417/ecc", "cmbPDFECC");
            set_radiobutton_from_setting(settings, "studio/bc/pdf417/encoding_mode", QStringList() << "radPDFStand" << "radPDFTruncated" << "radPDFHIBC");
            break;

        case BARCODE_MICROPDF417:
        case BARCODE_HIBC_MICPDF:
            set_combobox_from_setting(settings, "studio/bc/micropdf417/cols", "cmbMPDFCols");
            set_radiobutton_from_setting(settings, "studio/bc/micropdf417/encoding_mode", QStringList() << "radMPDFStand" << "radMPDFHIBC");
            break;

        case BARCODE_DOTCODE:
            set_combobox_from_setting(settings, "studio/bc/dotcode/cols", "cmbDotCols");
            set_radiobutton_from_setting(settings, "studio/bc/dotcode/encoding_mode", QStringList() << "radDotStand" << "radDotGS1");
            break;

        case BARCODE_AZTEC:
        case BARCODE_HIBC_AZTEC:
            set_radiobutton_from_setting(settings, "studio/bc/aztec/autoresizing", QStringList() << "radAztecAuto" << "radAztecSize" << "radAztecECC");
            set_combobox_from_setting(settings, "studio/bc/aztec/size", "cmbAztecSize");
            set_combobox_from_setting(settings, "studio/bc/aztec/ecc", "cmbAztecECC");
            set_radiobutton_from_setting(settings, "studio/bc/aztec/encoding_mode", QStringList() << "radAztecStand" << "radAztecGS1" << "radAztecHIBC");
            break;

        case BARCODE_MSI_PLESSEY:
            set_combobox_from_setting(settings, "studio/bc/msi_plessey/check_digit", "cmbMSICheck");
            break;

        case BARCODE_CODE11:
            set_radiobutton_from_setting(settings, "studio/bc/code11/check_digit", QStringList() << "radC11TwoCheckDigits" << "radC11OneCheckDigit" << "radC11NoCheckDigits");
            break;

        case BARCODE_CODE39:
        case BARCODE_HIBC_39:
            set_radiobutton_from_setting(settings, "studio/bc/code39/check_digit", QStringList() << "radC39Stand" << "radC39Check" << "radC39HIBC");
            break;

        case BARCODE_EXCODE39:
            set_radiobutton_from_setting(settings, "studio/bc/excode39/check_digit", QStringList() << "radC39Stand" << "radC39Check");
            break;

        case BARCODE_LOGMARS:
            set_radiobutton_from_setting(settings, "studio/bc/logmars/check_digit", QStringList() << "radLOGMARSStand" << "radLOGMARSCheck");
            break;

        case BARCODE_CODE16K:
            set_combobox_from_setting(settings, "studio/bc/code16k/row_sep_height", "cmbC16kRowSepHeight");
            set_radiobutton_from_setting(settings, "studio/bc/code16k/encoding_mode", QStringList() << "radC16kStand" << "radC16kGS1");
            break;

        case BARCODE_CODABAR:
            set_checkbox_from_setting(settings, "studio/bc/codabar/chk_check_char", "chkCodabarCheck");
            break;

        case BARCODE_CODABLOCKF:
        case BARCODE_HIBC_BLOCKF:
            set_combobox_from_setting(settings, "studio/bc/codablockf/width", "cmbCbfWidth");
            set_combobox_from_setting(settings, "studio/bc/codablockf/height", "cmbCbfHeight");
            set_combobox_from_setting(settings, "studio/bc/codablockf/row_sep_height", "cmbCbfRowSepHeight");
            set_radiobutton_from_setting(settings, "studio/bc/codablockf/encoding_mode", QStringList() << "radCbfStand" << "radCbfHIBC");
            break;

        case BARCODE_DATAMATRIX:
        case BARCODE_HIBC_DM:
            set_combobox_from_setting(settings, "studio/bc/datamatrix/size", "cmbDM200Size");
            set_radiobutton_from_setting(settings, "studio/bc/datamatrix/encoding_mode", QStringList() << "radDM200Stand" << "radDM200GS1" << "radDM200HIBC");
            set_checkbox_from_setting(settings, "studio/bc/datamatrix/chk_suppress_rect", "chkDMRectangle");
            set_checkbox_from_setting(settings, "studio/bc/datamatrix/chk_allow_dmre", "chkDMRE");
            set_checkbox_from_setting(settings, "studio/bc/datamatrix/chk_gs_sep", "chkDMGSSep");
            break;

        case BARCODE_QRCODE:
        case BARCODE_HIBC_QR:
            set_combobox_from_setting(settings, "studio/bc/qrcode/size", "cmbQRSize");
            set_combobox_from_setting(settings, "studio/bc/qrcode/ecc", "cmbQRECC");
            set_radiobutton_from_setting(settings, "studio/bc/qrcode/encoding_mode", QStringList() << "radDM200Stand" << "radQRGS1" << "radQRHIBC");
            set_checkbox_from_setting(settings, "studio/bc/qrcode/chk_full_multibyte", "chkQRFullMultibyte");
            break;

        case BARCODE_RMQR:
            set_combobox_from_setting(settings, "studio/bc/rmqr/size", "cmbRMQRSize");
            set_combobox_from_setting(settings, "studio/bc/rmqr/ecc", "cmbRMQRECC");
            set_radiobutton_from_setting(settings, "studio/bc/rmqr/encoding_mode", QStringList() << "radQRStand" << "radRMQRGS1");
            set_checkbox_from_setting(settings, "studio/bc/rmqr/chk_full_multibyte", "chkRMQRFullMultibyte");
            break;

        case BARCODE_HANXIN:
            set_combobox_from_setting(settings, "studio/bc/hanxin/size", "cmbHXSize");
            set_combobox_from_setting(settings, "studio/bc/hanxin/ecc", "cmbHXECC");
            set_checkbox_from_setting(settings, "studio/bc/hanxin/chk_full_multibyte", "chkHXFullMultibyte");
            break;

        case BARCODE_MICROQR:
            set_combobox_from_setting(settings, "studio/bc/microqr/size", "cmbMQRSize");
            set_combobox_from_setting(settings, "studio/bc/microqr/ecc", "cmbMQRECC");
            set_checkbox_from_setting(settings, "studio/bc/microqr/chk_full_multibyte", "chkMQRFullMultibyte");
            break;

        case BARCODE_GRIDMATRIX:
            set_combobox_from_setting(settings, "studio/bc/gridmatrix/size", "cmbGridSize");
            set_combobox_from_setting(settings, "studio/bc/gridmatrix/ecc", "cmbGridECC");
            set_checkbox_from_setting(settings, "studio/bc/gridmatrix/chk_full_multibyte", "chkGridFullMultibyte");
            break;

        case BARCODE_MAXICODE:
            set_combobox_from_setting(settings, "studio/bc/maxicode/mode", "cmbMaxiMode", 1);
            set_lineedit_from_setting(settings, "studio/bc/maxicode/primary_message", "txtMaxiPrimary", "Primary Message Here!");
            break;

        case BARCODE_CODEONE:
            set_combobox_from_setting(settings, "studio/bc/codeone/size", "cmbC1Size");
            set_radiobutton_from_setting(settings, "studio/bc/codeone/encoding_mode", QStringList() << "radC1Stand" << "radC1GS1");
            break;

        case BARCODE_CODE49:
            set_combobox_from_setting(settings, "studio/bc/code49/row_sep_height", "cmbC49RowSepHeight");
            set_radiobutton_from_setting(settings, "studio/bc/code49/encoding_mode", QStringList() << "radC49Stand" << "radC49GS1");
            break;

        case BARCODE_DBAR_EXPSTK:
            set_combobox_from_setting(settings, "studio/bc/dbar_expstk/cols", "cmbCols");
            break;

        case BARCODE_ULTRA:
            set_radiobutton_from_setting(settings, "studio/bc/ultra/autoresizing", QStringList() << "radUltraAuto" << "radUltraEcc");
            set_combobox_from_setting(settings, "studio/bc/ultra/ecc", "cmbUltraEcc");
            set_radiobutton_from_setting(settings, "studio/bc/ultra/encoding_mode", QStringList() << "radUltraStand" << "radUltraGS1");
            break;

        case BARCODE_UPCA:
        case BARCODE_UPCA_CHK:
        case BARCODE_UPCA_CC:
            set_combobox_from_setting(settings, "studio/bc/upca/addongap", "cmbUPCAAddonGap");
            break;

        case BARCODE_EANX:
        case BARCODE_EANX_CHK:
        case BARCODE_EANX_CC:
            set_combobox_from_setting(settings, "studio/bc/eanx/addongap", "cmbUPCEANAddonGap");
            break;

        case BARCODE_UPCE:
        case BARCODE_UPCE_CHK:
        case BARCODE_UPCE_CC:
            set_combobox_from_setting(settings, "studio/bc/upce/addongap", "cmbUPCEANAddonGap");
            break;

        case BARCODE_ISBNX:
            set_combobox_from_setting(settings, "studio/bc/isbnx/addongap", "cmbUPCEANAddonGap");
            break;

        case BARCODE_VIN:
            set_checkbox_from_setting(settings, "studio/bc/vin/chk_import_char_prefix", "chkVINImportChar");
            break;
    }
}
