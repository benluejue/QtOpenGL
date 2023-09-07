#include "light_control.h"

light_control::light_control(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::light_controlClass())
{
	ui->setupUi(this);
}

light_control::~light_control()
{
	delete ui;
}
