#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QStringList>
#include <QFileDialog>
#include <QInputDialog> 
#include <QLineEdit> 
#include <QRegularExpressionValidator> 
#include <QFile>        
#include <QTextStream>  
#include <QMessageBox>

#include "core-components/volemachine.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    VoleMachine machine;

    QMainWindow window;
    window.setWindowTitle("Vole Machine Simulator");

    QWidget *central = new QWidget(&window);
    window.setCentralWidget(central);

    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    // --- Top Toolbar (Buttons & Inputs) ---
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    QLabel *loadAddressLabel = new QLabel("Load at Address:");
    QLineEdit *loadAddressInput = new QLineEdit("00"); 
    loadAddressInput->setFixedWidth(50);
    loadAddressInput->setMaxLength(2); 

    QRegularExpression hexRegex("^[0-9a-fA-F]{1,2}$");
    QValidator *hexValidator = new QRegularExpressionValidator(hexRegex, loadAddressInput);
    loadAddressInput->setValidator(hexValidator);

    QPushButton *loadButton = new QPushButton("Load Program");
    QPushButton *stepButton = new QPushButton("Step");
    QPushButton *runButton = new QPushButton("Run All");
    QPushButton *resetButton = new QPushButton("Reset"); // Added Reset Button
    QPushButton *overwriteRegButton = new QPushButton("Overwrite Register");
    QPushButton *overwriteMemButton = new QPushButton("Overwrite Memory");

    buttonLayout->addWidget(loadAddressLabel);
    buttonLayout->addWidget(loadAddressInput);
    buttonLayout->addWidget(loadButton);
    
    buttonLayout->addSpacing(20); 
    
    buttonLayout->addWidget(stepButton);
    buttonLayout->addWidget(runButton);
    buttonLayout->addWidget(resetButton); // Placed next to Run All
    
    buttonLayout->addSpacing(20); 
    
    buttonLayout->addWidget(overwriteRegButton);
    buttonLayout->addWidget(overwriteMemButton);
    buttonLayout->addStretch();

    mainLayout->addLayout(buttonLayout);

    // --- Main Panels ---
    QHBoxLayout *contentLayout = new QHBoxLayout();

    // 1. Registers & Control Panel (Left)
    QWidget *registerPanel = new QWidget();
    QVBoxLayout *registerPanelLayout = new QVBoxLayout(registerPanel);

    QGroupBox *registerBox = new QGroupBox("Registers");
    QVBoxLayout *registerLayout = new QVBoxLayout(registerBox);

    QLabel *registerLabels[16];
    for (int i = 0; i < 16; i++) {
        registerLabels[i] = new QLabel(QString("R%1: 00").arg(i, 0, 16).toUpper());
        registerLabels[i]->setMinimumHeight(30);
        registerLabels[i]->setFrameShape(QFrame::Box);
        registerLabels[i]->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        registerLayout->addWidget(registerLabels[i]);
    }
    registerPanelLayout->addWidget(registerBox);

    QGroupBox *controlBox = new QGroupBox("Control");
    QVBoxLayout *controlLayout = new QVBoxLayout(controlBox);

    QLabel *pcLabel = new QLabel("PC: 00");
    QLabel *irLabel = new QLabel("IR: 00 00");
    pcLabel->setMinimumHeight(35);
    irLabel->setMinimumHeight(35);
    pcLabel->setFrameShape(QFrame::Box);
    irLabel->setFrameShape(QFrame::Box);

    controlLayout->addWidget(pcLabel);
    controlLayout->addWidget(irLabel);
    registerPanelLayout->addWidget(controlBox);

    // 2. Memory Panel (Center)
    QGroupBox *memoryBox = new QGroupBox("Memory (00 - FF)");
    QVBoxLayout *memoryLayout = new QVBoxLayout(memoryBox);
    QTableWidget *memoryTable = new QTableWidget(16, 16);

    memoryTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QStringList headers;
    for (int i = 0; i < 16; i++) {
        headers << QString::number(i, 16).toUpper();
    }
    memoryTable->setHorizontalHeaderLabels(headers);
    memoryTable->setVerticalHeaderLabels(headers);

    for (int row = 0; row < 16; row++) {
        for (int col = 0; col < 16; col++) {
            QTableWidgetItem *cell = new QTableWidgetItem("00");
            cell->setTextAlignment(Qt::AlignCenter);
            memoryTable->setItem(row, col, cell);
        }
    }
    memoryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    memoryTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    memoryLayout->addWidget(memoryTable);

    // 3. Program & Output Panels (Right side)
    QWidget *rightSidePanel = new QWidget();
    QVBoxLayout *rightSideLayout = new QVBoxLayout(rightSidePanel);
    rightSideLayout->setContentsMargins(0, 0, 0, 0);

    QGroupBox *programBox = new QGroupBox("Program");
    QVBoxLayout *programLayout = new QVBoxLayout(programBox);
    QTextEdit *programDisplay = new QTextEdit();
    programDisplay->setReadOnly(true);
    programLayout->addWidget(programDisplay);

    QGroupBox *outputBox = new QGroupBox("Screen Output (Address FF)");
    QVBoxLayout *outputLayout = new QVBoxLayout(outputBox);
    QTextEdit *outputDisplay = new QTextEdit();
    outputDisplay->setReadOnly(true);
    outputLayout->addWidget(outputDisplay);

    rightSideLayout->addWidget(programBox);
    rightSideLayout->addWidget(outputBox);

    contentLayout->addWidget(registerPanel, 1);
    contentLayout->addWidget(memoryBox, 3);
    contentLayout->addWidget(rightSidePanel, 2);

    mainLayout->addLayout(contentLayout);

    // --- Signals & Slots ---
    auto update_state = [&]() {
        for(int i = 0; i<16; i++){
            uint8_t val = machine.get_register(i);
            registerLabels[i]->setText(QString("R%1: %2")
                                        .arg(i, 0, 16)
                                        .arg(val, 2, 16, QChar('0')).toUpper());
        }

        memoryTable->blockSignals(true); 
        for (int row = 0; row < 16; row++) {
            for (int col = 0; col < 16; col++) {
                uint8_t address = static_cast<uint8_t>((row * 16) + col);  
                uint8_t value = machine.get_memory(address);
                memoryTable->item(row, col)->setText(QString("%1").arg(value, 2, 16, QChar('0')).toUpper());
            }
        }
        memoryTable->blockSignals(false);

        uint8_t pc = machine.get_pc(); 
        uint16_t ir = machine.get_ir();
        pcLabel->setText(QString("PC: %1").arg(pc, 2, 16, QChar('0')).toUpper());
        irLabel->setText(QString("IR: %1").arg(ir, 4, 16, QChar('0')).toUpper());
        
        QString screenStr = QString::fromStdString(machine.get_screen_buffer()); 
        outputDisplay->setText(screenStr);
    };



    QObject::connect(loadButton, &QPushButton::clicked, [&]() {
        QString addressText = loadAddressInput->text();
        if (addressText.isEmpty()) {            
            addressText = "00"; 
        }
        

        bool validHex;
        int loadAddress = addressText.toInt(&validHex, 16);

        if (validHex && loadAddress >= 0 && loadAddress <= 255) {
            QString fileName = QFileDialog::getOpenFileName(&window, "Open Program", "", "Text Files (*.txt)");
            if (!fileName.isEmpty()) {
                
                bool success = machine.load_program(fileName.toStdString(), static_cast<uint8_t>(loadAddress));
                
                if (!success) {
                    programDisplay->setText("ERROR: Invalid Vole instructions or user tried loading starting from an odd indexed memory cell. Load failed.");
                    return; 
                }
                
                QFile file(fileName);
                if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    QTextStream in(&file);
                    programDisplay->setText(in.readAll());
                    file.close();
                }

                update_state(); 
            }
        }
    });

    QObject::connect(stepButton, &QPushButton::clicked, [&]() {
        if (machine.get_error()) {
            QMessageBox::warning(&window, "Cannot Execute", "Machine is in an error state. Please load a new program.");
            return;
        }
        if (machine.get_halted()) {
            QMessageBox::information(&window, "Finished", "Machine is already halted. Please load a new program.");
            return;
        }

        machine.step();
        update_state();

        if (machine.get_error()) {
            QMessageBox::critical(&window, "Execution Error", "An invalid instruction or error occurred during execution!");
        } else if (machine.get_halted()) {
            QMessageBox::information(&window, "Halted", "Program executed the halt instruction (C000) successfully.");
        }
    });

    QObject::connect(runButton, &QPushButton::clicked, [&]() {
        if (machine.get_error()) {
            QMessageBox::warning(&window, "Cannot Execute", "Machine is in an error state. Please load a new program.");
            return;
        }
        if (machine.get_halted()) {
            QMessageBox::information(&window, "Finished", "Machine is already halted. Please load a new program.");
            return;
        }

        machine.run_all();
        update_state();     

        if (machine.get_error()) {
            QMessageBox::critical(&window, "Execution Error", "Program crashed due to an error during execution!");
        } else if (machine.get_halted()) {
            QMessageBox::information(&window, "Halted", "Program ran to completion and halted successfully.");
        }
    });

    // Reset Button Connection
    QObject::connect(resetButton, &QPushButton::clicked, [&]() {
        machine.reset();
        loadAddressInput->setText("00");
        programDisplay->clear();
        update_state();
        QMessageBox::information(&window, "Reset", "Simulator has been reset.");
    });

    QObject::connect(overwriteRegButton, &QPushButton::clicked, [&]() {
        bool ok1, ok2;
        QString regStr = QInputDialog::getText(&window, "Overwrite Register",
                                               "Enter Register Index (0 - F):", 
                                               QLineEdit::Normal, "", &ok1);
        if (ok1 && !regStr.isEmpty()) {
            QString valStr = QInputDialog::getText(&window, "Overwrite Register",
                                                   "Enter new value (00 - FF):", 
                                                   QLineEdit::Normal, "", &ok2);
            if (ok2 && !valStr.isEmpty()) {
                bool validReg, validVal;
                int index = regStr.toInt(&validReg, 16);
                int value = valStr.toInt(&validVal, 16);
                if(validReg && validVal && index<=15 && value<=255){
                    machine.overwrite_register(static_cast<uint8_t>(index), static_cast<uint8_t>(value));
                    update_state();
                }
            }
        }
    });
    
    QObject::connect(overwriteMemButton, &QPushButton::clicked, [&]() {
        bool ok1, ok2;
        QString addrStr = QInputDialog::getText(&window, "Overwrite Memory",
                                                "Enter Memory Address (00 - FF):", 
                                                QLineEdit::Normal, "", &ok1);
        if (ok1 && !addrStr.isEmpty()) {
            QString valStr = QInputDialog::getText(&window, "Overwrite Memory",
                                                   "Enter new value (00 - FF):", 
                                                   QLineEdit::Normal, "", &ok2);
            if (ok2 && !valStr.isEmpty()) {
                bool validAddr, validVal;
                int index = addrStr.toInt(&validAddr, 16);
                int value = valStr.toInt(&validVal, 16);

                if (validAddr && validVal && index <= 255 && value <= 255) {
                    machine.overwrite_memory(static_cast<uint8_t>(index), static_cast<uint8_t>(value));
                    update_state();
                }
            }
        }
    });

    window.resize(1200, 700); 
    window.showMaximized();

    return app.exec();
}