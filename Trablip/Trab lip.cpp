#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
using namespace std;
const int MAX = 100;//max para strings
const int IMAX = 512;//Max para tamanho
const int FMAX = 20;//Max filtro
struct RGB
{
    int R[IMAX][IMAX];
    int G[IMAX][IMAX];
    int B[IMAX][IMAX];
};
struct img
{
    RGB Img;
    int Height;
    int Width;
    int ColorMax;
    char Name[MAX];
    char Autenticator[MAX];
};
int Gray(int a, int b, int c);
void Preenchefiltro(int M[][FMAX],int i,int j);
void Open(char name[], img& Imagem,int& verificado,int& aberto);
void Format(char name[],int& verificado);
void SaveASS(char name[], img& Imagem);
void Binarizar (img Imagem_in, img& Imagem_out);
void Grayconverter (img Imagem_in, img& Imagem_out);
void Menu(img& Image_in,img& Image_out,int& exit,int& exe,char nomeI[],int& aberto);
void Selection(img& Image_in,img& Image_out,int& exe,int& exit,char nomeI[],int& aberto);
void Rotation(img Image_in, img& Image_out);
int Overwrite(char a[],char b[]);
void ErosionF(img Image_in, img& Image_out);
void Histograma(img Imagem_in, img& Imagem_out);
void zerar (int a[],int tam);
int somatorio(int a[],int indice);

int main()
{
    img Image_output,Image_input;
    int exit=0,exe=0,aberto=0;
    char Nomearq[MAX];
    do //Aplica varios efeitos em uma unica execu��o.
    {
        Menu(Image_input,Image_output,exit,exe,Nomearq,aberto);
    }
    while(!exit);
    return 0;
}
void Open(char name[], img& Imagem,int& verificado,int& aberto)
{
    ifstream img_in;
    img_in.open(name);
    if(img_in.is_open())
    {
        img_in.getline(Imagem.Autenticator,MAX);
        if(!strcmp(Imagem.Autenticator,"P3"))
        {
            img_in >> Imagem.Width;
            img_in >> Imagem.Height;
            img_in >> Imagem.ColorMax;
            for(int a=0; a<Imagem.Height; a++)
            {
                for(int l=0; l<Imagem.Width; l++)
                {
                    img_in >> Imagem.Img.R[l][a];
                    img_in >> Imagem.Img.G[l][a];
                    img_in >> Imagem.Img.B[l][a];
                }
            }
        }
        cout << "Arquivo aberto com Sucesso!"<< endl;
        cout << "Nome: " << name << " Altura: " << Imagem.Height<< " Largura: " << Imagem.Width << endl;
        verificado = 1;//caso aberto com sucesso, libera loop
        aberto=1;//libera filtros
    }
    else
    {
        verificado = 0;//nao libera loop do input da imagem
        cout << "Erro ao abrir o arquivo (Verifique se o nome esta correto!)" << endl;
    }
    img_in.close();
}
void SaveASS(char name[], img& Imagem)
{
    ofstream img_out;
    img_out.open(name);
    if(img_out.is_open())
    {
        img_out << Imagem.Autenticator << endl;
        img_out << Imagem.Width << " ";
        img_out << Imagem.Height << endl;
        img_out << Imagem.ColorMax << endl;
        for(int a=0; a<Imagem.Height; a++)
        {
            for(int l=0; l<Imagem.Width; l++)
            {
                img_out << Imagem.Img.R[l][a] << " ";
                img_out << Imagem.Img.G[l][a] << " ";
                img_out << Imagem.Img.B[l][a] << " ";
            }
            img_out << endl;
        }
        cout << "Arquivo salvo com Sucesso!"<< endl;
        cout << "Nome: " << name << " Altura: " << Imagem.Height<< " Largura: " << Imagem.Width << endl;
    }
    else
    {
        cout << "Erro ao salvar o arquivo" << endl;
        exit(0);
    }
    img_out.close();
}
void Binarizar (img Imagem_in, img& Imagem_out)
{
    Imagem_out=Imagem_in;
    int limiar=256,graypixel=0;
    do
    {
        cout << "   Entre com o limiar [0,255]" << endl;
        cin >> limiar;
        cin.ignore();
    }
    while(limiar>255||limiar<0);
    for(int a=0; a<Imagem_in.Height; a++)
    {
        for(int l=0; l<Imagem_in.Width; l++)
        {
            graypixel = (Imagem_in.Img.R[l][a]+Imagem_in.Img.G[l][a]+Imagem_in.Img.B[l][a])/3;
            if(graypixel >= limiar)
            {
                Imagem_out.Img.R[l][a]=255;
                Imagem_out.Img.G[l][a]=255;
                Imagem_out.Img.B[l][a]=255;
            }
            else
            {
                Imagem_out.Img.R[l][a]=0;
                Imagem_out.Img.G[l][a]=0;
                Imagem_out.Img.B[l][a]=0;
            }
            graypixel=0;
        }
    }
    cout << "Binarizacao realizada!" << endl;
}
void Grayconverter(img Imagem_in, img& Imagem_out)
{
    Imagem_out=Imagem_in;
    int graypixel=0;
    for(int a=0; a<Imagem_in.Height; a++)
    {
        for(int l=0; l<Imagem_in.Width; l++)
        {
            graypixel = Gray(Imagem_in.Img.R[l][a],Imagem_in.Img.G[l][a],Imagem_in.Img.B[l][a]);
            Imagem_out.Img.R[l][a]=graypixel;
            Imagem_out.Img.G[l][a]=graypixel;
            Imagem_out.Img.B[l][a]=graypixel;
        }
        graypixel=0;
    }
    cout << "Conversao realizada!" << endl;
}
void Format(char name[],int& verificado)
{
    int i =0,j=0,verified=0,verified2=0;
    char format[]=".ppm",formatoar[MAX];
    while(name[i]!='\0')
    {
        if(name[i]=='.')
        {
            while(name[i]!='\0')
            {
                formatoar[j]=name[i];
                i++;
                j++;
            }
            formatoar[j]='\0';
            if(!strcmp(formatoar,format))
            {
                verified=1;//caso o formato esteja de acordo, libera do loop na main
                break;
            }
            else
            {
                cout << "Formato nao suportado\n";
                cout << "Formato suportado: .ppm\n";
                verified2=1;
            }
        }
        i++;
    }
    verificado=verified;
    if(verified==0&&verified2==0)
    {
        strcat(name,format);
        verificado=1;
    }
}
void Menu(img& Image_in,img& Image_out,int& exit,int& exe,char nomeI[],int& aberto)
{
    cout << endl;
    cout << "Selecione quais processos deseja realizar: \n";
    cout << "   1. Binarizacao.\n";
    cout << "   2. Conv. Escala Cinza.\n";
    cout << "   3. Rotacao.\n";
    cout << "   4. Erosao.\n";
    cout << "   5. Histograma.\n";
    cout << "\n";
    cout << "   0. Abrir.\n";
    cout << "   -. Salvar.\n";
    cout << "   =. Salvar e sair.\n";
    Selection(Image_in,Image_out,exe,exit,nomeI,aberto);
    if(exe) //Seleciona o input de acordo com a execu��o.
    {
        Image_in=Image_out;
    }
    exe++;
}
void Selection(img& Image_in,img& Image_out,int& exe,int& exit,char nomeI[],int& aberto)
{
    int validacao=0;

    char opcao[MAX],Nomeresul[MAX],a[]="________________________________________________________________________________________________________________________";
    if(!aberto){
    cin.getline(opcao,MAX);
    cout << endl;
    switch (opcao[0])
    {
    case '0':
        cout << "\n";
        cout << a<<endl;
        cout << "   _Open_\n";
        cout << "Entre com o arquivo que voce deseja abrir:" << endl;
        do
        {
            cin.getline(nomeI,MAX);
            Format(nomeI,validacao);
            Open(nomeI,Image_in,validacao,aberto);
        }
        while(!validacao);
        exe=0;
        break;

        default:
        cout << "Voce precisa abrir uma imagem primeiro" << endl;
        opcao[0] = '0';
        Selection(Image_in,Image_out,exe,exit,nomeI,aberto);
        break;
    }
}
else{
    cin.getline(opcao,MAX);
    cout << endl;
        switch (opcao[0])
    {
        case '0':
        cout << "\n";
        cout << a<<endl;
        cout << "   _Open_\n";
        cout << "Entre com o arquivo que voce deseja abrir:" << endl;
        do
        {
            cin.getline(nomeI,MAX);
            Format(nomeI,validacao);
            Open(nomeI,Image_in,validacao,aberto);
        }
        while(!validacao);
        exe=0;
        break;

        case '-':
            cout << "\n";
            cout << a<<endl;
            cout << "   _Save_\n";
            do
            {
                do
                {
                    cout << "Digite o Nome final do arquivo:"<< endl;
                    cin.getline(Nomeresul,MAX);
                    Format(Nomeresul,validacao);//checa formato de output.
                }
                while(!validacao);
            }
            while(!Overwrite(Nomeresul,nomeI));
            SaveASS(Nomeresul,Image_out);
            break;

        case '=':
            cout << "\n";
            cout << a<<endl;
            cout << "   _Save & Exit_\n";
            do
            {
                do
                {
                    cout << "Digite o Nome final do arquivo:"<< endl;
                    cin.getline(Nomeresul,MAX);
                    Format(Nomeresul,validacao);//checa formato de output.
                }
                while(!validacao);
            }
            while(!Overwrite(Nomeresul,nomeI));
            SaveASS(Nomeresul,Image_out);
            exit=1;//libera do loop na main.
            break;

        case '1':
            cout << "   _Binarizacao_\n";
            Binarizar(Image_in,Image_out);
            cout << "\n";
            cout << a<<endl;
            break;

        case '2':
            cout << "   _Escala Cinza_\n";
            Grayconverter(Image_in,Image_out);
            cout << "\n";
            cout << a<<endl;
            break;
        case '3':
            cout << "   _Rotacao_\n";
            cout << "   1. Rotacao Horaria\n";
            cout << "   2. Rotacao Anti-Horaria\n";
            Rotation(Image_in,Image_out);
            cout << "\n";
            cout << a<<endl;
            break;
        case '4':
            cout << "   _Erosion_\n";
            ErosionF(Image_in,Image_out);
            cout << "\n";
            cout << a<<endl;
            break;
        case '5':
            cout << "   _Eq. Histograma_\n";
            Histograma(Image_in,Image_out);
            cout << "\n";
            cout << a<<endl;
            break;
    default:
        cout << "Opcao invalida! " << opcao[0] << endl;
        Selection(Image_in,Image_out,exe,exit,nomeI,aberto);
        break;
    }
}
}
int Overwrite(char a[],char b[])
{
    ifstream over;
    over.open(a);

    char res[MAX];
    if(over.is_open()){
            cout << "Ja existe um arquivo com esse nome, deseja substituir? (y/n)" << endl;
        cin.getline(res,MAX);
        if(res[0]=='y')
            return 1;
        else if (res[0]=='n')
            return 0;
    }
    else{
        return 1;
    }
    over.close();
}

void Rotation(img Image_in, img& Image_out)
{
    int s1=0,s2=0,sel=0;
    char rot[MAX];
    strcpy(Image_out.Autenticator,Image_in.Autenticator);
    Image_out.ColorMax=Image_in.ColorMax;
    do{
        cin.getline(rot,MAX);
    if(rot[0]=='1')//faz a sele��o do tipo de rota��o por meio da anula��o.
    {
        cout << "Horaria\n";
        s1=1;
        sel=1;
    }
    else if(rot[0]=='2')
    {
        cout << "Anti-horaria\n";
        s2=1;
        sel=1;
    }
    else {
        cout << "Opcao invalida!\n";
        sel=0;
    }
    }while(!sel);
    Image_out.Height=Image_in.Width;
    Image_out.Width=Image_in.Height;
    for(int i=0; i<Image_in.Width; i++)
    {
        for(int j=0; j<Image_in.Height; j++)
        {
            Image_out.Img.R[j][i]=Image_in.Img.R[((Image_in.Width-1)-i)*s2+i*s1][((Image_in.Height-1)-j)*s1+j*s2];
            Image_out.Img.G[j][i]=Image_in.Img.G[((Image_in.Width-1)-i)*s2+i*s1][((Image_in.Height-1)-j)*s1+j*s2];
            Image_out.Img.B[j][i]=Image_in.Img.B[((Image_in.Width-1)-i)*s2+i*s1][((Image_in.Height-1)-j)*s1+j*s2];
        }

    }

}
void ErosionF(img Image_in, img& Image_out)
{
    int Filter[FMAX][FMAX],tamx=2,tamy=2;
    Preenchefiltro(Filter,tamx,tamy);
    Image_out=Image_in;
    for(int a=0; a<Image_in.Height-1; a++)
    {
        for(int l=0; l<Image_in.Width-1; l++)
        {
            if((Gray(Image_in.Img.R[l][a],Image_in.Img.G[l][a],Image_in.Img.B[l][a])==Filter[0][0])&&(Gray(Image_in.Img.R[l+1][a+1],Image_in.Img.G[l+1][a+1],Image_in.Img.B[l+1][a+1])==Filter[1][1])&&(Gray(Image_in.Img.R[l+1][a],Image_in.Img.G[l+1][a],Image_in.Img.B[l+1][a])==Filter[1][0])&&(Gray(Image_in.Img.R[l][a+1],Image_in.Img.G[l][a+1],Image_in.Img.B[l][a+1])==Filter[0][1]))
            {
                Image_out.Img.R[l][a]=255;
                Image_out.Img.G[l][a]=255;
                Image_out.Img.B[l][a]=255;
            }
            else
            {
                Image_out.Img.R[l][a]=0;
                Image_out.Img.G[l][a]=0;
                Image_out.Img.B[l][a]=0;
            }
        }
    }
    cout << "Erosao realizada!" << endl;
}
void Preenchefiltro(int M[][FMAX],int tamx,int tamy)
{
    for (int i=0; i<tamx; i++)
    {
        for (int j=0; j<tamy; j++)
        {
            M[i][j]=255;
        }
    }
}
int Gray(int a, int b, int c)
{
    return((a+b+c)/3);
}

void Histograma(img Image_in, img& Image_out)
{
    Image_out=Image_in;
    float s=((Image_in.ColorMax)/float(Image_in.Height*Image_in.Width));
    int repR[(Image_in.ColorMax)+1],repG[(Image_in.ColorMax)+1],repB[(Image_in.ColorMax)+1];
    zerar(repR,(Image_in.ColorMax)),zerar(repG,(Image_in.ColorMax)),zerar(repB,(Image_in.ColorMax));
    for(int i=0; i<Image_in.Height; i++)
    {
        for(int j=0; j<Image_in.Width; j++)
        {
            repR[(Image_in.Img.R[i][j])]+=1;
            repG[(Image_in.Img.G[i][j])]+=1;
            repB[(Image_in.Img.B[i][j])]+=1;
        }
    }
    for(int i=0; i<Image_in.Height; i++)
    {
        for(int j=0; j<Image_in.Width; j++)
        {
            Image_out.Img.R[i][j]=s*somatorio(repR,Image_in.Img.R[i][j]);
            Image_out.Img.G[i][j]=s*somatorio(repG,Image_in.Img.G[i][j]);
            Image_out.Img.B[i][j]=s*somatorio(repB,Image_in.Img.B[i][j]);
        }

    }
    cout << "Equalizacao realizada!" << endl;
}

void zerar (int a[],int tam)
{
    for (int i=0; i<=tam; i++)
        a[i]=0;
}

int somatorio(int a[],int indice)
{
    int soma=0;
    for(int i=0; i<=indice; i++)
    {
        soma+=a[i];
    }
    return(soma);

}
