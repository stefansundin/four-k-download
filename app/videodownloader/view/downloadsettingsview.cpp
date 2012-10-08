/// Copyright 2010-2012 4kdownload.com (developers@4kdownload.com)
/**
    This file is part of 4k Download.

    4k Download is free software; you can redistribute it and/or modify
    it under the terms of the one of two licenses as you choose:

    1. GNU GENERAL PUBLIC LICENSE Version 3
    (See file COPYING.GPLv3 for details).

    2. 4k Download Commercial License
    (Send request to developers@4kdownload.com for details).
   
*/


#include "downloadsettingsview.h"
#include "ui_downloadsettingsview.h"
#include <view/mediaitemview.h>
#include <view/mediadetailsview.h>
#include <QDesktopServices>
#include <QUrl>
#include <QMovie>

using namespace View;
using namespace ViewModel;
using namespace ComponentModel;
using namespace Gui;
using namespace Mvvm;


DownloadSettingsView::DownloadSettingsView(const Factory* factory, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DownloadSettingsView),
    m_movie(NULL),
    m_acceptButton(NULL),
    m_factory(factory),
    m_menu(this)
{
    ui->setupUi(this);

    QFont font = ui->parsingLabel->font();
#if defined(Q_OS_WIN)
    QFontInfo fontInfo(font);
    font.setFamily("Segoe UI");
    font.setPixelSize(fontInfo.pixelSize() + 1);
#endif
    font.setBold(true);
    ui->parsingLabel->setFont(font);

    QColor color = ui->delimiterLine->palette().color(QPalette::Window).darker(112);
    QString stylesheet = QString("background-color: white; color: rgb(%1, %2, %3);").arg(
                QString::number(color.red()),
                QString::number(color.green()),
                QString::number(color.blue())
                );
    ui->delimiterLine->setStyleSheet(stylesheet);
    ui->delimiterLine_2->setStyleSheet(stylesheet);
    ui->videoLine->setStyleSheet(stylesheet);
    ui->subtitlesLine->setStyleSheet(stylesheet);
    ui->audioLine->setStyleSheet(stylesheet);
    ui->saveLine->setStyleSheet(stylesheet);
    ui->delimiterLine_2->setVisible(false);

    m_movie = new QMovie(":/image/progress", QByteArray(), this);
    m_movie->start();
    ui->progressLabel->setMovie(m_movie);

    ui->buttonBox->addButton(tr("Cancel"), QDialogButtonBox::RejectRole);
    m_acceptButton = ui->buttonBox->addButton(tr("Download"), QDialogButtonBox::AcceptRole);
    m_acceptButton->setEnabled(false);

    ui->parsingWidget->setVisible(true);
    ui->mediaScrollArea->setVisible(false);
    ui->qualityTitleWidget->setVisible(false);
    ui->qualityWidget->setVisible(false);
    ui->extractTitleWidget->setVisible(false);
    ui->extractWidget->setVisible(false);
    ui->subtitlesTitleWidget->setVisible(false);
    ui->subtitlesWidget->setVisible(false);
    ui->saveTitleWidget->setVisible(false);
    ui->saveWidget->setVisible(false);

    QLayout* layout = new QVBoxLayout(ui->mediaScrollArea->widget());
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    ui->mediaScrollArea->widget()->setLayout(layout);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QObject::connect(ui->saveEdit, SIGNAL(editingFinished()),
                     this, SLOT(saveEditingFinished()));
    QObject::connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)),
                     this, SLOT(buttonBoxClicked(QAbstractButton*)));
}


DownloadSettingsView::~DownloadSettingsView()
{
    delete ui;
}


QObject* DownloadSettingsView::viewModel() const
{
    return m_viewModel.data();
}


void DownloadSettingsView::setViewModel(QObject* value)
{
    if (m_viewModel.data() != value)
    {
        if (m_viewModel)
        {
            m_menu.clear();

            m_subtitlesEnabled1Binding.reset();
            m_subtitlesEnabled2Binding.reset();
            m_subtitlesDownloadBinding.reset();
            m_subtitlesBinding.reset();
            m_fileBinding.reset();
            m_fileActionBinding.reset();

            QObject::disconnect(this, SLOT(propertyChanged(ComponentModel::PropertyChangedSignalArgs)));
        }


        m_viewModel = dynamic_cast<DownloadSettingsViewModel*>(value);


        if (m_viewModel && m_factory)
        {
            QList<QAction*> list;
            list.append(m_viewModel.data()->checkAllAction());
            list.append(m_viewModel.data()->uncheckAllAction());
            m_menu.addActions(list);

            m_subtitlesEnabled1Binding.reset(new PropertyBinding(ui->subtitlesTitleWidget, "enabled", m_viewModel.data(), "subtitlesEnabled"));
            m_subtitlesEnabled2Binding.reset(new PropertyBinding(ui->subtitlesWidget, "enabled", m_viewModel.data(), "subtitlesEnabled"));
            m_subtitlesDownloadBinding.reset(new PropertyBinding(ui->subtitlesCheckBox, "checked", m_viewModel.data(), "subtitlesDownload", PropertyBinding::TwoWay));
            m_subtitlesBinding.reset(new ComboboxBinding(ui->subtitlesComboBox, m_viewModel.data(), "subtitlesList", "subtitlesIndex"));
            m_fileBinding.reset(new PropertyBinding(ui->saveEdit, "text", m_viewModel.data(), "outputPath"));
            m_fileActionBinding.reset(new ButtonActionBinding(ui->saveButton, m_viewModel.data()->outputPathAction()));

            QObject::connect(m_viewModel.data(), SIGNAL(propertyChanged(ComponentModel::PropertyChangedSignalArgs)),
                             this, SLOT(propertyChanged(ComponentModel::PropertyChangedSignalArgs)));
        }
    }
}


bool hasExtractAudio(QList< QSharedPointer<ViewModel::MediaDetailsViewModel> > detailsList)
{
    for (int i = 0; i < detailsList.count(); ++i)
    {
        if (detailsList[i]->downloadType() == ViewModel::ExtractAudio)
            return true;
    }

    return false;
}

void DownloadSettingsView::propertyChanged(const ComponentModel::PropertyChangedSignalArgs& args)
{
    if (args.property() == "state")
    {
        DownloadSettingsViewModel::State state = m_viewModel.data()->state();

        if (state == DownloadSettingsViewModel::Error)
        {
            setResult(QDialog::Accepted);
            close();
        }

        if (state == DownloadSettingsViewModel::Done)
            m_movie->stop();

        ui->parsingWidget->setVisible(state != DownloadSettingsViewModel::Done);
        ui->delimiterLine_2->setVisible(state == DownloadSettingsViewModel::Retreiving);
        ui->mediaScrollArea->setVisible(state == DownloadSettingsViewModel::Retreiving ||
                                        state == DownloadSettingsViewModel::Done);
        ui->qualityTitleWidget->setVisible(state == DownloadSettingsViewModel::Done);
        ui->qualityWidget->setVisible(state == DownloadSettingsViewModel::Done);
        ui->extractTitleWidget->setVisible(state == DownloadSettingsViewModel::Done &&
                                           hasExtractAudio(m_viewModel.data()->detailsList()));
        ui->extractWidget->setVisible(state == DownloadSettingsViewModel::Done &&
                                      hasExtractAudio(m_viewModel.data()->detailsList()));
        ui->subtitlesTitleWidget->setVisible(state == DownloadSettingsViewModel::Done &&
                                             m_viewModel.data()->subtitlesList().count() > 0);
        ui->subtitlesWidget->setVisible(state == DownloadSettingsViewModel::Done &&
                                        m_viewModel.data()->subtitlesList().count() > 0);
        ui->saveTitleWidget->setVisible(state == DownloadSettingsViewModel::Done);
        ui->saveWidget->setVisible(state == DownloadSettingsViewModel::Done);

        if (state == DownloadSettingsViewModel::Done && m_viewModel.data()->detailsList().count() > 0)
        {
            m_acceptButton->setEnabled(true);
            m_acceptButton->setDefault(true);
        }
    }
    else if (args.property() == "mediaList")
    {
        QList< QSharedPointer<ViewModel::MediaItemViewModel> > mediaList = m_viewModel.data()->mediaList();

        for (int i = 0; i < mediaList.count(); ++i)
        {
            MediaItemView * v = new MediaItemView(m_factory.data());
            v->setViewModel(mediaList[i].data());
            v->setContextMenuPolicy(Qt::CustomContextMenu);
            QObject::connect(v, SIGNAL(customContextMenuRequested(QPoint)),
                             this, SLOT(contextMenuRequest(QPoint)));
            ui->mediaScrollArea->widget()->layout()->addWidget(v);
            v->show();

            if (i == 0)
            {
                v->setCheckedVisible(mediaList.count() > 1);
                ui->mediaScrollArea->setFixedHeight(v->sizeHint().height() * qMin(mediaList.count(), 4));
            }

            v->setOdd(i % 2);
        }

        setWindowTitle(mediaList.count() > 1 ? tr("Download Playlist") : tr("Download Clip"));
    }
    else if (args.property() == "detailsList")
    {
        QList< QSharedPointer<ViewModel::MediaDetailsViewModel> > detailsList = m_viewModel.data()->detailsList();
        int detailsIndex = m_viewModel.data()->detailsIndex();

        for (int i = 0; i < detailsList.count(); ++i)
        {
            MediaDetailsView * v = new MediaDetailsView(m_factory.data());
            v->setViewModel(detailsList[i].data());
            v->setChecked(i == detailsIndex);
            QObject::connect(v, SIGNAL(toggled(bool)), this, SLOT(radioButtonToggled(bool)));

            if (detailsList[i]->downloadType() == ViewModel::DownloadVideo)
                ui->qualityWidget->layout()->addWidget(v);
            else
                ui->extractWidget->layout()->addWidget(v);
        }
    }
    else if (args.property() == "detailsIndex")
    {
        QLayout* layoutQuality = ui->qualityWidget->layout();
        QLayout* layoutExtract = ui->extractWidget->layout();
        int detailsIndex = m_viewModel.data()->detailsIndex();

        for (int i = 0; i < layoutQuality->count() + layoutExtract->count(); ++i)
        {
            MediaDetailsView * v = (MediaDetailsView*)(i < layoutQuality->count() ? layoutQuality->itemAt(i)->widget() : layoutExtract->itemAt(i - layoutQuality->count())->widget());
            v->setChecked(i == detailsIndex);
        }
    }
}


void DownloadSettingsView::radioButtonToggled(bool toggled)
{
    if (!toggled)
        return;

    QLayout* layoutQuality = ui->qualityWidget->layout();
    QLayout* layoutExtract = ui->extractWidget->layout();

    for (int i = 0; i < layoutQuality->count() + layoutExtract->count(); ++i)
    {
        QWidget * v = i < layoutQuality->count() ? layoutQuality->itemAt(i)->widget() : layoutExtract->itemAt(i - layoutQuality->count())->widget();

        if (sender() == v)
        {
            m_viewModel.data()->setDetailsIndex(i);
            break;
        }
    }

    if (m_acceptButton)
    {
        QList< QSharedPointer<ViewModel::MediaDetailsViewModel> > detailsList = m_viewModel.data()->detailsList();
        int detailsIndex = m_viewModel.data()->detailsIndex();

        m_acceptButton->setText(detailsList[detailsIndex]->downloadType() == ViewModel::DownloadVideo ? tr("Download") : tr("Extract"));
    }
}


void DownloadSettingsView::saveEditingFinished()
{
    if (m_viewModel)
        m_viewModel.data()->setOutputPath(ui->saveEdit->text());
}


void DownloadSettingsView::buttonBoxClicked(QAbstractButton* button)
{
    if (m_viewModel)
        m_viewModel.data()->setOutputPath(ui->saveEdit->text());

    if (ui->buttonBox->buttonRole(button) == QDialogButtonBox::AcceptRole)
    {
        if (m_viewModel.data()->canClose())
            accept();
    }
    else
        reject();
}


void DownloadSettingsView::contextMenuRequest(const QPoint& point)
{
    if (m_viewModel && !m_viewModel.data()->checkAllAction()->isEnabled())
        return;

    m_menu.exec(qobject_cast<QWidget*>(sender())->mapToGlobal(point));
}
