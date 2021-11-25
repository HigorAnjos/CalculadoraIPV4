#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Funcoes.cpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit_ipv4Binario->setEnabled(false);
    ui->lineEdit_ipv4Decimal->setEnabled(false);
    ui->lineEdit_mascaraBinario->setEnabled(false);
    ui->lineEdit_mascaraDecimal->setEnabled(false);
    ui->lineEdit_subReds->setEnabled(false);
    ui->textEdit->setReadOnly(true);
    ui->lineEditCIDR->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_radioButton_binario_toggled(bool checked)
{
    if((!ui->lineEdit_ipv4Binario->isEnabled() && !ui->lineEdit_mascaraBinario->isEnabled()) || (ui->lineEdit_ipv4Decimal->isEnabled() && ui->lineEdit_mascaraDecimal->isEnabled())){
        ui->lineEdit_ipv4Binario->setEnabled(true);
        ui->lineEdit_mascaraBinario->setEnabled(true);
        ui->lineEdit_ipv4Decimal->setEnabled(false);
        ui->lineEdit_mascaraDecimal->setEnabled(false);
        ui->lineEdit_subReds->setEnabled(true);
        ui->lineEditCIDR->setEnabled(false);
    }
}


void MainWindow::on_radioButton_decimal_toggled(bool checked)
{
        ui->lineEdit_ipv4Binario->setEnabled(false);
        ui->lineEdit_mascaraBinario->setEnabled(false);
        ui->lineEdit_ipv4Decimal->setEnabled(true);
        ui->lineEdit_mascaraDecimal->setEnabled(true);
        ui->lineEdit_subReds->setEnabled(true);
        ui->lineEditCIDR->setEnabled(false);

}


void MainWindow::on_pushButton_calcular_clicked()
{
try {
        //entrada -- CIDR
        //Ultimo endereço da ultima rede ta errado
        //por . a cada 8bits em binario
        QString saida;
        int numRedes;
        int* ipv4 = new int[4];
        int* mascara = new int[4];
        int* ipBinario = new int[32];
        int* mascBinario = new int[32];


        if(ui->lineEdit_subReds->text() == "") throw QString ("Preencha o campo SUBREDES");
        numRedes = ui->lineEdit_subReds->text().toInt();

        if(ui->radioButton_binario->isChecked())
        {
            if(ui->lineEdit_ipv4Binario->text() == "") throw QString ("Preencha o campo IPV4 BINARIO");
            if(ui->lineEdit_mascaraBinario->text() == "") throw QString ("Preencha o campo MASCARA BINARIO");


            QStringList b = ui->lineEdit_ipv4Binario->text().split("");

            for(int i = 1; i <= 32; i++)
            {
                ipBinario[i - 1] = b[i].toInt();
            }
            //11000000101010000110010000000000 C
            //11111111111111111111111100000000 C
            //192.168.100.0 C

            //11111111000000000000000000000000 A

            ipDecimalMontador(ipBinario, ipv4);

            QStringList m = ui->lineEdit_mascaraBinario->text().split("");

            for(int i = 1; i <= 32; i++)
            {
                mascBinario[i - 1] = m[i].toInt();
            }

            ipDecimalMontador(mascBinario, mascara);
            numeroDeSubRedesPossiveis(mascBinario, numRedes);

            saida += "IPV4 digitado em decimal : " + QString::number(ipv4[0]) + "." +  QString::number(ipv4[1]) + "." + QString::number(ipv4[2]) + "." + QString::number(ipv4[3]) + "\n";
            saida += "IPV4 digitado em binario : " + ui->lineEdit_ipv4Binario->text() + "\n";
            saida += "Mascara digitada em decimal : " + QString::number(mascara[0]) + "." +  QString::number(mascara[1]) + "." + QString::number(mascara[2]) + "." + QString::number(mascara[3]) + "\n";
            saida += "Mascara digitada em binario : " + ui->lineEdit_mascaraBinario->text() + "\n";

        }
        else if (ui->radioButton_decimal->isChecked()){
                if(ui->lineEdit_ipv4Decimal->text() == "") throw QString ("Preencha o campo IPV4 DECIMAL");
                if(ui->lineEdit_mascaraDecimal->text() == "") throw QString ("Preencha o campo MASCARA DECIMAL");

                QStringList a;
                a = ui->lineEdit_ipv4Decimal->text().split(".");

                ipv4[0] = a[0].toInt();
                ipv4[1] = a[1].toInt();
                ipv4[2] = a[2].toInt();
                ipv4[3] = a[3].toInt();

                QStringList b;
                b = ui->lineEdit_mascaraDecimal->text().split(".");

                mascara[0] = b[0].toInt();
                mascara[1] = b[1].toInt();
                mascara[2] = b[2].toInt();
                mascara[3] = b[3].toInt();

                int* aux = retornaVetorzao(mascara[0], mascara[1], mascara[2], mascara[3]);
                numeroDeSubRedesPossiveis(aux, numRedes);

                int* aux2 = retornaVetorzao(ipv4[0], ipv4[1], ipv4[2], ipv4[3]);

                saida += "IPV4 digitado em decimal : " + ui->lineEdit_ipv4Decimal->text() + "\n";
                saida += "IPV4 digitado em binario : ";
                for(int i = 0; i < 32; i++)
                {
                    saida += QString::number(aux2[i]);
                    if((i + 1 )%8 == 0 && i < 30){saida+= ".";}
                }
                saida += "\n";
                saida += "Mascara digitada em decimal : " + ui->lineEdit_mascaraDecimal->text() + "\n";
                saida += "Mascara digitada em binario : ";
                for(int i = 0; i < 32; i++)
                {
                    saida += QString::number(aux[i]);
                    if((i + 1 )%8 == 0 && i < 30){saida+= ".";}
                }
                saida += "\n";
            }

            else if(ui->radioButton->isChecked())//RadioButtonCIDR - CIDR
            {
                int cidr = ui->lineEditCIDR->text().toInt();
                int* mascaraCidr= new int[32];

                QStringList c = ui->lineEdit_ipv4Decimal->text().split(".");

                ipv4[0] = c[0].toInt();
                ipv4[1] = c[1].toInt();
                ipv4[2] = c[2].toInt();
                ipv4[3] = c[3].toInt();

                saida += "Mascara em binario : ";
                for(int i = 0; i < 32; i++)
                {
                    mascaraCidr[i] = 0;
                    if(i < cidr)
                    {
                        mascaraCidr[i] = 1;
                    }

                    saida += QString::number(mascaraCidr[i]);
                }



                ////////////////////////////////////////
                /// Pegando a rede do ip digitado
                //vet -> mascara
                //ip [32]
                int* auxIp;
                auxIp = retornaVetorzao(ipv4[0], ipv4[1], ipv4[2], ipv4[3]);



                ///validando IPV4 cidr
                vaidandoIpv4(auxIp, cidr);

                //Endereço da rede
                redeDoIp(auxIp, mascaraCidr);

                /// auxIp -> Decimal
                ipDecimalMontador(auxIp, ipv4);
                //////Saida
                saida += "\nEndereço da rede : " +  QString::number(ipv4[0])+ "." + QString::number(ipv4[1])+ "." + QString::number(ipv4[2])+ "." + QString::number(ipv4[3]);
                //////////////////////////////////////////


                ipDecimalMontador(mascaraCidr, mascara);


                saida += "\n Mascara digitada em decimal : " + QString::number(mascara[0]);
                saida += "." + QString::number(mascara[1]);
                saida += "." + QString::number(mascara[2]);
                saida += "." + QString::number(mascara[3]);
                subRedeMontador(numRedes, mascara, ipv4);
                //-------------------------------
            }

            saida += "\n\n";

          ///Fomatando a saida
          SaidaQdtring(saida, numRedes, ipv4, mascara, ipBinario, mascBinario);

          ui->textEdit->setText(saida);

    } catch (QString &erro)
        {
            QMessageBox::information(this, "erro", erro);
        }

}


void MainWindow::on_pushButton_limpar_clicked()
{
    ui->lineEdit_ipv4Binario->clear();
    ui->lineEdit_ipv4Decimal->clear();
    ui->lineEdit_mascaraBinario->clear();
    ui->lineEdit_mascaraDecimal->clear();
    ui->lineEdit_subReds->clear();
    ui->textEdit->clear();
}

void MainWindow::on_radioButton_clicked()
{
    ui->lineEdit_ipv4Decimal->setEnabled(true);
    ui->lineEdit_subReds->setEnabled(true);
    ui->lineEdit_ipv4Binario->setEnabled(false);
    ui->lineEdit_mascaraBinario->setEnabled(false);
    ui->lineEdit_mascaraDecimal->setEnabled(false);
    ui->lineEditCIDR->setEnabled(true);
}
