#include <QtWidgets>

#include "dialog.h"

Dialog::Dialog()
{
	createMenu();
	createPathBar();
	createHorizontalGroupBox();
	createBottomButtons();

	main_layout = new QVBoxLayout;
	main_layout->setMenuBar(menuBar);
	main_layout->addLayout(path_line_layout);
	main_layout->addLayout(total_button_layout);
	main_layout->addWidget(buttonBox);
	setLayout(main_layout);

	setWindowTitle(tr("Teeth Annotation Tool @ Oculus Research Pittsburgh"));
}

void Dialog::createMenu()
{
	menuBar = new QMenuBar;

	fileMenu = new QMenu(tr("&File"), this);
	openAction = fileMenu->addAction(tr("&Open"));
	exitAction = fileMenu->addAction(tr("&Exit"));
	menuBar->addMenu(fileMenu);

	connect(openAction, SIGNAL(triggered()), this, SLOT(openFolder()));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(reject()));
}

void Dialog::createHorizontalGroupBox()
{
	buttons = new singleTeethBox*[config::h_directions.size() * config::v_directions.size() * numButtons];
	total_button_layout = new QGridLayout;
	QString result_fd = getResultFolder(_now_dir);
	for (int i = 0; i < config::v_directions.size(); i++) {
		for (int j = 0; j < config::h_directions.size(); j++) {
			std::string group_title = config::v_directions[i] + ' ' + config::h_directions[j];
			QGroupBox* quater_group_box = new QGroupBox(tr(group_title.c_str()));
			QHBoxLayout* quater_layout = new QHBoxLayout;
			for (int k = 0; k < config::part_names.size(); k++) {
				std::string part_str = config::v_directions[i] + ' ' + config::h_directions[j] + ' ' + config::part_names[k];
				singleTeethBox* this_button = new singleTeethBox(NULL, part_str);
				buttons[i * (config::h_directions.size()*config::part_names.size()) + j * (config::part_names.size()) + k] = this_button;
				quater_layout->addWidget(this_button);
			}
			quater_group_box->setLayout(quater_layout);
			total_button_layout->addWidget(quater_group_box, i, j);
		}
	}
	return;
}

void Dialog::createPathBar() 
{
	path_line_layout = new QHBoxLayout;
	path_label = new QLabel("Current Path:");
	path_line_layout->addWidget(path_label);
	path_line = new QLineEdit(config::path_line_string.c_str());
	path_line->setReadOnly(true);
	path_line_layout->addWidget(path_line);
}

void Dialog::createBottomButtons()
{
	buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void Dialog::openFolder()
{
	now_dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
		"C:\\Users\\shangxuanu\\Dropbox (Personal)\\Oculus\\teeth\\sample_scan",
		QFileDialog::ShowDirsOnly);
	if (isValidFolder(now_dir))
	{
		path_line->setText(now_dir);
	}
	else
	{
		path_line->setText(config::wrong_path_line_string.c_str());
	}
	QString result_dir = getResultFolder(now_dir);
	for (int i = 0; i < config::h_directions.size() * config::v_directions.size() * numButtons; i++)
	{
		buttons[i]->setResultFd(result_dir.toStdString());
	}
	mkdirIfMissing(result_dir);
	_now_dir = now_dir;
}