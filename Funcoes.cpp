#include <iostream>
#include <QString>
#include <math.h>

struct subRede
{

    ///subRede
    int* iPBinSub = new int [32];
    int* ipDecimalSub = new int [4];

    //Mascara da rede
    int* ipMascaraBin = new int [32];
    int* ipMascaraDescimal = new int [4];

    ///BroadCast
    int* iPBinBroad = new int [32];
    int* ipDecimalBroad= new int [4];

    //Primeriro host (Maquina ip)
    int* ipBinPrimeiroValido = new int [32];
    int* ipDecimalPrimeiroValido = new int [4];

    ///Ultimo host
    int* ipBinUltimoValido = new int [32];
    int* ipDecimalUltimoValido = new int [4];

    ///Quantidade de mascara nessa sub rede
    long long maquinasNaSubRede = 0;

};

int* transformaBinario(int num);
bool interarIpBinario(int* ip, int inicio, int fim);

///////////Valida IP

int  bitsRedeClasse(int * ip)
{

    if(ip[0] == 0)
    {
        return 8; // Classe A
    }

    if(ip[0] == 1 && ip[1] == 0)
    {
        return 16; // Classe B
    }

    if(ip[0] == 1 && ip[1] == 1 && ip[2] == 0)
    {
        return 24; // Classe C
    }

    return -1;

}

bool vaidandoIpv4(int * ip, int cidr)
{
    //Descobrindo bits host (Classe)
    int BITShostAteAqui = bitsRedeClasse(ip);

    if(BITShostAteAqui == -1)
    {
        QString e = "Erro! ao validar a classe ! "  + QString::number(cidr);
        throw QString(e);
    }


    if(cidr < BITShostAteAqui)
    {
        QString e = "Pois o cidr precisa ser igual ou maior a Classe da rede cidr > " + QString::number(BITShostAteAqui) + "!";
        throw QString(e);
        return false;
    }

    return true;


}





///Validando ip e Mascara

bool zerosMesmaPosicaoMacaraIpBin(int* mascaraBin, int* ipBin)
{
    int i = 0;
    for(i=0; i<32 && mascaraBin[i] == 1; i++)
    {}

    for(i ; i<32 && ipBin[i] == 0; i++)
    {}

    if(i == 32)
    {
        return true;
    }
    return false;
}


bool validaIpMascara(int* mascaraBin, int* ipBin)
{
    if(zerosMesmaPosicaoMacaraIpBin(mascaraBin,ipBin) )
    {
        return true;

    }

    throw QString("Nao possui os bits zeros no ip como na mascara");

    return false;
}

bool validaDec(int x, int y, int z, int t)
{
    if(x>=0 && x<=255)
    {
        if(y>=0 && y<=255)
        {
            if(z>=0 && z<=255)
            {
                if(t>=0 && t<=255)
                {
                    return true;
                }
            }
        }

    }

    throw QString("Fora do intervalo [0, .., 255]");

    return false;

}
///////////
void novaMascara(int* oldMascara, int* subMascara, int qtdBits)
{

    int i = 0;

    for(i=0; i<32; i++)
    {
        subMascara[i] = oldMascara[i];
    }

    for(i=0; i<32 && oldMascara[i] == 1; i++){}

    for(i; qtdBits; qtdBits--, i++)
    {
        subMascara[i] = 1;
    }




}

int* retornaVetorzao(int x, int y, int z, int t)
{
    int* vetorzao = new int [32];
    int* vetX;
    int* vetY;
    int* vetZ;
    int* vetT;
    vetX = transformaBinario(x);
    vetY = transformaBinario(y);
    vetZ = transformaBinario(z);
    vetT = transformaBinario(t);
    int indice = 0;
    int i =0;

    for(i=0; i<=7; i++)
    {
        vetorzao[i] = vetX[i];
    }
    indice = i;
    for(i=0, indice; i<=7; indice++,i++)
    {
        vetorzao[indice] = vetY[i];
    }
    for(i=0, indice; i<=7; indice++,i++)
    {
        vetorzao[indice] = vetZ[i];
    }
    for(i=0, indice; i<=7; indice++,i++)
    {
        vetorzao[indice] = vetT[i];
    }

    return vetorzao;

}


int* transformaBinario(int num)
{
    int i = 7;
    int* vet = new int[8];
    while(i >= 0)
    {
         vet[i] = num % 2;
         num = num / 2;
         i--;
    }

    /*for(int x = 0; x < 8; x++)
    {
        cout << vet[x];
    }*/
    return vet;
}



float bitsParaSubRede(int numSub)
{
    return ceil(log2(numSub));
}



//Ip Binario para decimal
int ipDecimal(int* ipBinario, int inicio, int fim)
{
    int i=0, exp=0;
    int decimal = 0;
    for(i=fim; inicio<=i; i-- )
    {
        decimal += pow(2,exp) * ipBinario[i];
        exp++;
    }
    return decimal;
}
void ipDecimalMontador(int* ipBinario, int* decimal)
{
    decimal[0] = ipDecimal(ipBinario, 0, 7);
    decimal[1] = ipDecimal(ipBinario, 8, 15);
    decimal[2] = ipDecimal(ipBinario, 16, 23);
    decimal[3] = ipDecimal(ipBinario, 24, 31);
}
/// Ip Binario para decimal - Fim

int zeroMascara(int* mascara32)
{
    int i = 0;
    for(i=0; i<32 && mascara32[i] == 1; i++){}

    if(i != 32)
    {
        return i;
    }
    return -1; //Nao tem 0 nessa mascara
}

bool numeroDeSubRedesPossiveis(int* mascara, int qntRedes)
{
    int posicao = zeroMascara(mascara);
    int maximo = 32 - posicao;
    if(qntRedes <= 0) throw QString("O numero de sub-redes deve ser maior que zero");
    int aux = pow(2,(maximo - 2));
    QString a = "Acima da quantidade maxima de : " + QString::number(aux) + " sub-redes";

    if(qntRedes > aux) throw a;
    if(qntRedes > 0 && qntRedes < aux)
    {
        return true;
    }
    return false;
}



bool interarIpBinario(int* ip, int inicio, int fim)
{
    fim--; // corrigindo a posicao final
    int aux = 1;
    int i = 0;

    for(i=fim; inicio<= i; i--)
    {
        aux+= ip[i];
        if(aux == 2)
        {
            aux = 1;
            ip[i] = 0;
            continue;
        }else
        {
            ip[i] = aux;
            aux = 0;
            break;
        }
    }
    if(aux != 0)
    {
        return false;
    }
    return true;

}
bool decrementarIpBinario (int* ip, int inicio, int fim)
{
    fim--;
    int aux = -1;
    int i = 0;

    for(i=fim; inicio<= i; i--)
    {
        aux+= ip[i];
        if(aux == -1)
        {
            ip[i] = 1;
            continue;
        }else
        {
            ip[i] = 0;
            break;
        }
    }

    if(i == inicio-1)
    {
        return false;
    }
    return true;
}


subRede* subRedeMontador (int qtdSubRede, int* mascara, int* ip)
{
    //mascara [4] x.y.z.t , ip[4] x.y.z.t
    int particoesSubRede = pow(2, bitsParaSubRede(qtdSubRede));
    subRede* sRede = new subRede [particoesSubRede];


    int* mascaraBinario = retornaVetorzao(mascara[0], mascara[1], mascara[2], mascara[3]); //Retorna endereco em binario int[32]
    int* ipBinario = retornaVetorzao(ip[0], ip[1], ip[2], ip[3]);

    int qtdBitsSubRede = bitsParaSubRede(qtdSubRede);
    int posicaoASomarBit = zeroMascara(mascaraBinario); //inicio
    int posicaoASomarBitFinal =  posicaoASomarBit + qtdBitsSubRede; // possui erro de posicao

    int quantidadeMaquinasPorSubRede = pow(2,32-posicaoASomarBit-qtdBitsSubRede)-2;

    int i = 0, j = 0;


    bool ok = true; // so pa garantir
    for(i=0; i<particoesSubRede && ok;  i++)
    {
        for(j=0; j<32; j++)
        {
            sRede[i].iPBinSub[j] = ipBinario[j];
            sRede[i].ipBinPrimeiroValido[j] = ipBinario[j];
        }
        //Primerira maquina
        interarIpBinario(sRede[i].ipBinPrimeiroValido,0,32);


        //Proxima subRede
        ok = interarIpBinario(ipBinario, posicaoASomarBit - 1, posicaoASomarBitFinal);

        //Endereco da prox sub rede -1, e o broad cast
        for(j=0; j<32; j++)
        { // pegando o ip da prox rede
            sRede[i].iPBinBroad[j] = ipBinario[j];
        }
        // decrementa 1, para BroadCast
        decrementarIpBinario(sRede[i].iPBinBroad, 0, 32);

        ///Ultimo Host ip valido
        ///Pega o broad cast e decrementa 1
        for(j=0; j<32; j++)
        { // pegando o ip da prox rede
            sRede[i].ipBinUltimoValido[j] = sRede[i].iPBinBroad[j];
        }
        // decrementa 1, para BroadCast
        decrementarIpBinario(sRede[i].ipBinUltimoValido, 0, 32);
    }

    //Pegando no struct as subRedes em binario e inserindo no struct em Decimal
    for(i=0; i<particoesSubRede;  i++)
    {
      ipDecimalMontador(sRede[i].iPBinSub, sRede[i].ipDecimalSub);
    }

    //Pegando no struct Primeiro host em binario e inserindo no struct host em Decimal
    for(i=0; i<particoesSubRede;  i++)
    {
      ipDecimalMontador(sRede[i].ipBinPrimeiroValido, sRede[i].ipDecimalPrimeiroValido);
    }

    for(i=0; i<particoesSubRede;  i++)
    {
       novaMascara(mascaraBinario, sRede[i].ipMascaraBin, qtdBitsSubRede);
       ipDecimalMontador(sRede[i].ipMascaraBin, sRede[i].ipMascaraDescimal);
    }

    //Pegando no struct BroadCast em binario e inserindo no struct BroadCast em Decimal
    for(i=0; i<particoesSubRede;  i++)
    {
      ipDecimalMontador(sRede[i].iPBinBroad, sRede[i].ipDecimalBroad);
    }

    //Pegando no struct Ultimo host em binario e inserindo no struct Ultimo em Decimal
    for(i=0; i<particoesSubRede;  i++)
    {
      ipDecimalMontador(sRede[i].ipBinUltimoValido, sRede[i].ipDecimalUltimoValido);
    }

    ///Quantidade de maquinas por sub rede
    for(i=0; i<particoesSubRede;  i++)
    {
      sRede[i].maquinasNaSubRede = quantidadeMaquinasPorSubRede;
    }

    delete mascaraBinario;
    delete ipBinario;
    return sRede;

}

void redeDoIp(int* ip, int* mascara)
{
    int bitClasse =  bitsRedeClasse(ip);

    int i = 0;
    for(i=bitClasse; mascara[i] == 1 && i<32 ; i++)
    {}

    for(i; i<32; i++)
    {
        ip[i] = 0;
    }

}
///Formatando a saida
void SaidaQdtring(QString& saida, int numRedes, int* ipv4, int* mascara, int* ipBinario, int* mascBinario)
{
    int partcoesSub = pow(2,bitsParaSubRede(numRedes));

    subRede* sredes = subRedeMontador(numRedes, mascara, ipv4);

    int i = 0;

    saida += "Numero de maquinas em cada sub-rede : " + QString::number(sredes[0].maquinasNaSubRede) + "\n";
    saida += "--------------------------------------------------------------------------------------------------------------\n\n";

    for(i=0; i< numRedes; i++)
    {
        saida +=  "Sub-Rede " + QString::number(i + 1) + " em binario : ";
        for(int j=0; j<32; j++)
        {
            saida += QString::number(sredes[i].iPBinSub[j]);
            if((j + 1 )%8 == 0 && j < 30){saida+= ".";}

        }
        saida += "\n";

        saida += "Sub-Rede " + QString::number(i + 1) + " em decimal : ";
        saida += QString::number(sredes[i].ipDecimalSub[0]) + "." + QString::number(sredes[i].ipDecimalSub[1]) + "." + QString::number(sredes[i].ipDecimalSub[2]) + "." + QString::number(sredes[i].ipDecimalSub[3]) + "\n";

        saida += "\n";


        saida +=  "Mascara Sub-Rede " + QString::number(i + 1) + " em binario : ";
        for(int j=0; j<32; j++)
        {
            saida += QString::number(sredes[i].ipMascaraBin[j]);
            if((j + 1 )%8 == 0 && j < 30){saida+= ".";}

        }
        saida += "\n";

        saida += "Mascara Sub-Rede " + QString::number(i + 1) + " em decimal : ";
        saida += QString::number(sredes[i].ipMascaraDescimal[0]) + "." + QString::number(sredes[i].ipMascaraDescimal[1]) + "." + QString::number(sredes[i].ipMascaraDescimal[2]) + "." + QString::number(sredes[i].ipMascaraDescimal[3]) + "\n";

        saida += "\n";



        saida += "Primeiro host da sub-rede " + QString::number(i + 1) + " em binario : ";
        for(int j=0; j<32; j++)
        {
             saida += QString::number(sredes[i].ipBinPrimeiroValido[j]);
             if((j + 1 )%8 == 0 && j < 30){saida+= ".";}
        }
        saida += "\n";
        saida += "Primeiro host da sub-rede " + QString::number(i + 1) + " em decimal : " +  QString::number(sredes[i].ipDecimalPrimeiroValido[0]) + "." + QString::number(sredes[i].ipDecimalPrimeiroValido[1]) + "." + QString::number(sredes[i].ipDecimalPrimeiroValido[2]) + "." + QString::number(sredes[i].ipDecimalPrimeiroValido[3]) + "\n";

        saida += "\n";
        saida += "Ultimo host valido da sub-rede " + QString::number(i + 1) + " em binario : ";
        for(int j=0; j<32; j++)
        {
             saida +=  QString::number(sredes[i].ipBinUltimoValido[j]);
             if((j + 1 )%8 == 0 && j < 30){saida+= ".";}
        }
        saida += "\n";
        saida += "Ultimo host da sub-rede " + QString::number(i + 1) + " em decimal : " + QString::number(sredes[i].ipDecimalUltimoValido[0]) + "." + QString::number(sredes[i].ipDecimalUltimoValido[1]) + "." + QString::number(sredes[i].ipDecimalUltimoValido[2]) + "." + QString::number(sredes[i].ipDecimalUltimoValido[3]);
        saida += "\n";



        saida += "\nBroadCast  da rede " + QString::number(i + 1) + " em binario : ";
        for(int j=0; j<32; j++)
        {
             saida += QString::number(sredes[i].iPBinBroad[j]);
             if((j + 1 )%8 == 0 && j < 30){saida+= ".";}
        }
        saida += "\n";
        saida += "Broadcast da sub-rede " + QString::number(i + 1) + " em decimal : " + QString::number(sredes[i].ipDecimalBroad[0]) + "." + QString::number(sredes[i].ipDecimalBroad[1]) + "." + QString::number(sredes[i].ipDecimalBroad[2]) + "." + QString::number(sredes[i].ipDecimalBroad[3]);
        saida += "\n";

        saida += "--------------------------------------------------------------------------------------------------------------\n";

    }

    //------------------------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------------------------

    saida += "--------------------------------------------------------------------------------------------------------------\n\n";
    saida += "REDES OCIOSAS : " + QString::number(partcoesSub - numRedes) + "\n\n";

    for( ; i< partcoesSub; i++)
    {
        saida +=  "Sub-Rede " + QString::number(i + 1) + " em binario : ";
        for(int j=0; j<32; j++)
        {
            saida += QString::number(sredes[i].iPBinSub[j]);
            if((j + 1 )%8 == 0 && j < 30){saida+= ".";}
        }
        saida += "\n";

        saida += "Sub-Rede " + QString::number(i + 1) + " em decimal : ";
        saida += QString::number(sredes[i].ipDecimalSub[0]) + "." + QString::number(sredes[i].ipDecimalSub[1]) + "." + QString::number(sredes[i].ipDecimalSub[2]) + "." + QString::number(sredes[i].ipDecimalSub[3]) + "\n";

        saida += "\n";

        saida += "Primeiro host da sub-rede " + QString::number(i + 1) + " em binario : ";
        for(int j=0; j<32; j++)
        {
             saida += QString::number(sredes[i].ipBinPrimeiroValido[j]);
             if((j + 1 )%8 == 0 && j < 30){saida+= ".";}
        }
        saida += "\n";
        saida += "Primeiro host da sub-rede " + QString::number(i + 1) + " em decimal : " +  QString::number(sredes[i].ipDecimalPrimeiroValido[0]) + "." + QString::number(sredes[i].ipDecimalPrimeiroValido[1]) + "." + QString::number(sredes[i].ipDecimalPrimeiroValido[2]) + "." + QString::number(sredes[i].ipDecimalPrimeiroValido[3]) + "\n";

        saida += "\n";
        saida += "Ultimo host valido da sub-rede " + QString::number(i + 1) + " em binario : ";
        for(int j=0; j<32; j++)
        {
             saida +=  QString::number(sredes[i].ipBinUltimoValido[j]);
             if((j + 1 )%8 == 0 && j < 30){saida+= ".";}
        }
        saida += "\n";
        saida += "Ultimo host da sub-rede " + QString::number(i + 1) + " em decimal : " + QString::number(sredes[i].ipDecimalUltimoValido[0]) + "." + QString::number(sredes[i].ipDecimalUltimoValido[1]) + "." + QString::number(sredes[i].ipDecimalUltimoValido[2]) + "." + QString::number(sredes[i].ipDecimalUltimoValido[3]);
        saida += "\n";



        saida += "\nBroadCast  da rede " + QString::number(i + 1) + " em binario : ";
        for(int j=0; j<32; j++)
        {
             saida += QString::number(sredes[i].iPBinBroad[j]);
             if((j + 1 )%8 == 0 && j < 30){saida+= ".";}
        }
        saida += "\n";
        saida += "Broadcast da sub-rede " + QString::number(i + 1) + " em decimal : " + QString::number(sredes[i].ipDecimalBroad[0]) + "." + QString::number(sredes[i].ipDecimalBroad[1]) + "." + QString::number(sredes[i].ipDecimalBroad[2]) + "." + QString::number(sredes[i].ipDecimalBroad[3]);
        saida += "\n";

        saida += "--------------------------------------------------------------------------------------------------------------\n";

    }



}



