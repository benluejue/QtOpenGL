#pragma once

#include <QWidget>
#include "ui_light_control.h"

QT_BEGIN_NAMESPACE
namespace Ui { class light_controlClass; };
QT_END_NAMESPACE

class light_control : public QWidget
{
	Q_OBJECT

public:
	light_control(QWidget *parent = nullptr);
	~light_control();

private:
	Ui::light_controlClass *ui;
};
