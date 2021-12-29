#pragma once

#include <QMainWindow>
#include <poppler/qt5/poppler-qt5.h>

QT_BEGIN_NAMESPACE
  namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class CalibrationDialog;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;
  MainWindow(const MainWindow&) = delete;
  MainWindow(MainWindow&&) = delete;
  MainWindow& operator=(const MainWindow&) = delete;
  MainWindow& operator=(MainWindow&&) = delete;
  bool open_document(const QString& filename);
  void zoom(int increment);

private Q_SLOTS:
  void on_action_Open_triggered();
  void on_action_Exit_triggered();
  void on_sp_page_valueChanged(int page);
  void on_pb_calibrate_clicked();
  void on_cb_calibration_currentIndexChanged(int index);
  void on_pb_zoom_in_clicked();
  void on_pb_zoom_out_clicked();

private:
  std::size_t m_zoom = 4;
  std::unique_ptr<Ui::MainWindow> m_ui;
  std::unique_ptr<CalibrationDialog> m_calibration_dialog;
  std::unique_ptr<Poppler::Document> m_doc;
};
