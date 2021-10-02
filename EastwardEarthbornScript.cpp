#include <stdio.h>
#include <iostream>
#include <easyx.h>
#include <windows.h>
#include <string>
#include<cmath>

RECT rect;

BYTE scan_code(DWORD pKey)//�����ź�ת��ɨ����,ͻ����Ϸ��������̵�����
{
    const DWORD result = MapVirtualKey(pKey, MAPVK_VK_TO_VSC);
    return static_cast<BYTE>(result);
}

void presskey(DWORD pKey)
{
    keybd_event(static_cast<BYTE>(pKey), scan_code(pKey), 0, 0);
    Sleep(100);
    keybd_event(static_cast<BYTE>(pKey), scan_code(pKey), KEYEVENTF_KEYUP, 0);
    Sleep(100);
}

bool isSame(IMAGE* pimg1,IMAGE* pimg2,int x,int y)
{
    for(int i=0;i<x;i++)
    {
        for (int j = 0; j < y; j++)
        {
            SetWorkingImage(pimg1);
            COLORREF c1 = getpixel(i, j);
            SetWorkingImage(pimg2);
            COLORREF c2 = getpixel(i, j);
            int B = abs(BYTE GetBValue(c1) - BYTE GetBValue(c2));
            int R = abs(BYTE GetRValue(c1) - BYTE GetRValue(c2));
            if (B > 80 || R > 80)//�����һ����һ��
            { 
                return false;
            }
        }
    }
    return true;
}

bool isObject(IMAGE O[5], IMAGE N[5])//�ж��Ƿ�ΪĿ��ְҵ���
{
    for (int i = 0; i < 5; i++)
    {
        if (!isSame(&O[i], &N[i], 46, 46))//ֻҪ��һ����һ��
        {
            return false;
        }
            
    }
    return true;
}

void reset()//�ؿ�
{
    //����TAB
    presskey(9);
    //����s��
    presskey('S');
    //����s��
    presskey('S');
    //����s��
    presskey('S');
    //����s��
    presskey('S');
    //���¿ո�
    presskey(' ');

    Sleep(7500);//������¿�ʼ���7.5s
    //���¿ո�
    presskey(' ');

    Sleep(10000);
    for (int i = 0; i < 24; i++)
    {
        //���¿ո�
        presskey(' ');
        Sleep(1000);
    }
}

using std::cout; using std::cin;
using std::endl; using std::string;
int main()
{
    const char* name[] = { "ʥ����ʿ.jpg","������ɮ.jpg","��������.jpg","�Ľ߷�ʦ.jpg","�������.jpg" };//Ŀ��ְҵ����ͼ����ļ���
    IMAGE imgO[5];//Ŀ��ְҵͼ��
    IMAGE imgN[5];//��ǰְҵͼ���ȡ
    IMAGE image(1920, 1080);//��Ϸ����
    IMAGE imgNull;//��ͼ
    IMAGE imgFlag;//ֹͣ���Ի��ı�־
    HWND gamehwnd = FindWindow("SDL_app", "Eastward");  //������ܴ���ID�ı���
    HDC gamehdc = GetDC(gamehwnd); //���gamehwnd��Ϸ���ڵ�ͼ��
    HDC imghdc = GetImageHDC(&image);

    loadimage(&imgFlag, "flag.jpg");

    for (int i = 0; i < 5; i++)
    {
        loadimage(&imgO[i], name[i]);
    }

    SetForegroundWindow(gamehwnd);//����Ϸ���ڷ����������ǰ��
    Sleep(2000);

    while (true)//��ͣ����
    {
        BitBlt(imghdc, 0, 0, 1920, 1080, gamehdc, 0, 0, SRCCOPY);//����Ϸ����ͼ��gamehdcת��ͼƬ����imghdc
        SetWorkingImage(&image);//�����и�ͼƬ�Ĺ�����
        getimage(&imgN[0], 838, 745, 46, 46);//��ʿ��ְҵͼ���ȡ

        if (!isSame(&imgN[0], &imgO[0], 46, 46))
        {
            reset();
            continue;
        }

        SetForegroundWindow(gamehwnd);//����Ϸ���ڷ����������ǰ��
        //����s��
        presskey('S');

        //���¿ո�
        presskey(' ');

        //����D��
        presskey('D');

        //����D��
        presskey('D');

        for (int i = 1; i < 5; i++)//�������ְҵͼ��
        {
            BitBlt(imghdc, 0, 0, 1920, 1080, gamehdc, 0, 0, SRCCOPY);//����Ϸ����ͼ��gamehdcת��ͼƬ����imghdc
            SetWorkingImage(&image);//�����и�ͼƬ�Ĺ�����
            getimage(&imgN[i], 1401, 271, 46, 46);
            //����D��
            presskey('D');
        }
        if (isObject(imgO, imgN))//�ж��Ƿ�ΪĿ��ְҵ���
            break;

        reset();
    }

    return 0;
}