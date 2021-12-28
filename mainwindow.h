#pragma once

#include <QMainWindow>
#include <QPdfDocument>

QT_BEGIN_NAMESPACE
  namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  bool open_document(const QString& filename);

private Q_SLOTS:
  void on_action_Open_triggered();
  void on_action_Exit_triggered();
  void on_sp_page_valueChanged(int page);
  void on_sp_zoom_valueChanged(double exponent);

private:
  std::unique_ptr<Ui::MainWindow> m_ui;
  QPdfDocument m_doc;
};
