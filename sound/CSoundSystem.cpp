

#define INITGUID

//#include "stdafx.h"
#include "../DexBase/typedefine.h"
#include "CSoundSystem.h"
#include "../state/DexGameEngine.h"


std::string CSound::m_soundPath = "sound/";
CSound::CSound()
{
	ZeroMemory(m_pDirectSoundBuffer, sizeof(LPDIRECTSOUNDBUFFER8) * 4);
	m_pDirectSound = NULL;
	m_volume = 0;                     //0Ϊԭ�����������������
}

CSound::~CSound()
{
	ShutDown();
}
void CSound::ShutDown()
{
	for (int i = 0; i < 4; i++)
		if(m_pDirectSoundBuffer[i])
		{
			m_pDirectSoundBuffer[i]->Release();
			m_pDirectSoundBuffer[i] = NULL;
		}
	if(m_pDirectSound)
	{
		m_pDirectSound->Release();
		m_pDirectSound = NULL;
	}
}

bool CSound::CreateBuffer(char *filename, int index)
{
    FILE *fp;
    if((fp=fopen(filename, "rb"))==NULL)
	{
		_Message("�ļ�%s��ʧ��!��Ϣ��CSound::CreateBuffer(char *filename, int index)", filename);
        return false;
	}
    LPDIRECTSOUNDBUFFER pDirectBuffer;

	sWaveHeader header;
    fseek(fp, 0, SEEK_SET);
    fread(&header, 1, sizeof(sWaveHeader), fp);
    if(memcmp(header.RiffSig, "RIFF", 4)   ||                     
	   memcmp(header.WaveSig, "WAVE", 4)   ||                      
	   memcmp(header.FormatSig, "fmt ", 4) ||                   
	   memcmp(header.DataSig, "data",4))
	{
		_Message("sWaveHeader��Ϣ��������!��Ϣ��CSound::CreateBuffer(char *filename, int index)");
        return false;
	} 
    DSBUFFERDESC dsbd;
    WAVEFORMATEX wfex;

    ZeroMemory(&wfex, sizeof(WAVEFORMATEX));
    wfex.wFormatTag = WAVE_FORMAT_PCM;
    wfex.nChannels = header.Channels;
    wfex.nSamplesPerSec = header.SampleRate;
    wfex.wBitsPerSample = header.BitsPerSample;
    wfex.nBlockAlign = wfex.wBitsPerSample / 8 * wfex.nChannels;
    wfex.nAvgBytesPerSec = wfex.nSamplesPerSec * wfex.nBlockAlign;

    ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
    dsbd.dwSize = sizeof(DSBUFFERDESC);
    dsbd.dwFlags = DSBCAPS_CTRLVOLUME;
    dsbd.dwBufferBytes = header.DataSize;
    dsbd.lpwfxFormat = &wfex;
    if(FAILED(m_pDirectSound->CreateSoundBuffer(&dsbd, &pDirectBuffer, NULL)))
	{
		_Message("����������ʧ��!��Ϣ��CSound::CreateBuffer(char *filename, int index)");
        return false;
	} 
    if(FAILED(pDirectBuffer->QueryInterface(IID_IDirectSoundBuffer8,     
				   (void**)&m_pDirectSoundBuffer[index]))) 
	{
		_Message("�������ӿڲ�ѯʧ��!��Ϣ��CSound::CreateBuffer(char *filename, int index)");
        return false;
	} 
    pDirectBuffer->Release();		 //�ͷţ���ֹ�ڴ�й¶
	if(!CopyFileData(fp, header.DataSize, index))
		return false;
	fclose(fp);			             //�ر��ļ�
	return true;
}		
	
bool CSound::CopyFileData(FILE * file, int size, int index)
{
    BYTE *Ptr1, *Ptr2;
    DWORD Size1, Size2;
    if(!size)
	{
		_Message("�����ļ���С����!��Ϣ��CSound::CopyFileData(FILE * file, int size, int index)");
        return false;
	} 
    if(FAILED(m_pDirectSoundBuffer[index]->Lock(0, size, (void**)&Ptr1, &Size1, (void**)&Ptr2, &Size2, 0)))
        return false;

    fread(Ptr1, 1, Size1, file);
    if(Ptr2 != NULL)
      fread(Ptr2, 1, Size2, file);
    m_pDirectSoundBuffer[index]->Unlock(Ptr1, Size1, Ptr2, Size2);
    return true;
}

bool CSound::Initial()
{
  if(FAILED(DirectSoundCreate8(NULL, &m_pDirectSound, NULL)))
  {
      MessageBox(NULL, "Unable to create DirectSound object", "Error", MB_OK);
      return false;
  }
  m_pDirectSound->SetCooperativeLevel(DexGameEngine::getEngine()->GetHwnd(), DSSCL_NORMAL);
  return true;
}
void CSound::Play(char *filename, int index, bool loop)
{
	string name = m_soundPath + string(filename);
	ReleaseBuffer(index);                          //�����ͷ�֮ǰ����Դ�����������ڴ�й¶
	if(!CreateBuffer((char*)name.c_str(), index))			   //���뻺������Դ
		return;
	DWORD flag;
	if(loop)
		flag = DSBPLAY_LOOPING;	
	else 
		flag = 0;
	m_pDirectSoundBuffer[index]->SetCurrentPosition(0);
	m_pDirectSoundBuffer[index]->SetVolume(m_volume);
	m_pDirectSoundBuffer[index]->Play(0, 0, flag);
}

void CSound::Play(int index, bool loop)
{
	if(m_pDirectSoundBuffer[index] == NULL)
		return;
	DWORD flag;
	if(loop)
		flag = DSBPLAY_LOOPING;	
	else 
		flag = 0;
	m_pDirectSoundBuffer[index]->SetCurrentPosition(0);
	m_pDirectSoundBuffer[index]->SetVolume(m_volume);
	m_pDirectSoundBuffer[index]->Play(0, 0, flag);
}

void CSound::SetVolume( long volume )
{
	if(volume > 100 || volume < 0)
		return;
	m_volume = volume * 100 - 10000;      //��100~0��0~-10000����ֵת��
}

void CSound::Stop( int index )
{
	if(m_pDirectSoundBuffer[index] == NULL)
		return;
	m_pDirectSoundBuffer[index]->Stop();
}

void CSound::ReleaseBuffer( int index )
{
	if(m_pDirectSoundBuffer[index] == NULL)
		return;
	m_pDirectSoundBuffer[index]->Release();   
	m_pDirectSoundBuffer[index] = NULL;
}
CMusicMidi::CMusicMidi()
{
	m_bStop = true;
}

bool CMusicMidi::Initial( )
{	
	m_pDirectMusicPerformance = NULL;
	m_pDirectMusicLoader      = NULL;
	m_pDirectMusicSegment     = NULL;
	m_time                    = 0;
	m_volume                  = -200;
	CoInitialize(0);      //��ʼ��COM
    CoCreateInstance(CLSID_DirectMusicLoader, NULL, CLSCTX_INPROC, 
                   IID_IDirectMusicLoader8, (void**)&m_pDirectMusicLoader);

    CoCreateInstance(CLSID_DirectMusicPerformance, NULL, CLSCTX_INPROC, 
                   IID_IDirectMusicPerformance8, (void**)&m_pDirectMusicPerformance);

    m_pDirectMusicPerformance->InitAudio(NULL, NULL, DexGameEngine::getEngine()->GetHwnd(),
                              DMUS_APATH_SHARED_STEREOPLUSREVERB, 128,
                              DMUS_AUDIOF_ALL , NULL);
    char strPath[MAX_PATH];
    WCHAR wstrSearchPath[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, strPath);
    MultiByteToWideChar(CP_ACP, 0, strPath, -1, wstrSearchPath, MAX_PATH);
    m_pDirectMusicLoader->SetSearchDirectory(GUID_DirectMusicAllTypes, wstrSearchPath, FALSE);
	return true;
}

void CMusicMidi::Play(char *filename, bool loop)
{
	if(m_pDirectMusicSegment != NULL)
	{
		m_pDirectMusicSegment->Release();   //���֮ǰ���������ͷţ������ڴ�й¶
	    m_pDirectMusicSegment = NULL;
		m_pDirectMusicLoader->ClearCache(GUID_DirectMusicAllTypes);	   //�������
	} 
	if(m_pDirectMusicPerformance->IsPlaying(m_pDirectMusicSegment, NULL))	 //���֮ǰ�ڲ�����ֹͣ������������������,
		m_pDirectMusicPerformance->StopEx(m_pDirectMusicSegment, 0, 0);	 //������������g_pDMSegment�ͷ�֮����У������������

    DMUS_OBJECTDESC dmod;

    ZeroMemory(&dmod, sizeof(DMUS_OBJECTDESC));
    dmod.dwSize = sizeof(DMUS_OBJECTDESC);
    dmod.guidClass = CLSID_DirectMusicSegment;
    dmod.dwValidData = DMUS_OBJ_CLASS | DMUS_OBJ_FILENAME | DMUS_OBJ_FULLPATH ;
    mbstowcs(dmod.wszFileName, filename, MAX_PATH);
    if(FAILED(m_pDirectMusicLoader->GetObject(&dmod, IID_IDirectMusicSegment8, (LPVOID*)&m_pDirectMusicSegment)))
	{
		_Message("m_pDirectMusicLoader����ʧ��!��Ϣ��CMusicMidi::Play(char *filename, bool loop)");
        return ;
	} 

    if(strstr(filename, ".mid") != NULL)
        if(FAILED(m_pDirectMusicSegment->SetParam(GUID_StandardMIDIFile, -1, 0, 0, (void *)m_pDirectMusicPerformance)))
		{
			_Message("m_pDirectMusicSegment����ʧ��!��Ϣ��CMusicMidi::Play(char *filename, bool loop)");
			return ;
		}
    m_pDirectMusicSegment->SetParam(GUID_Download, -1, 0, 0, (void *)m_pDirectMusicPerformance);


    if(FAILED(m_pDirectMusicSegment->Download(m_pDirectMusicPerformance)))
	{
		_Message("m_pDirectMusicSegment����ʧ��!��Ϣ��CMusicMidi::Play(char *filename, bool loop)");
        return ;
	} 

	DWORD flag;
	if(loop)
		flag = DMUS_SEG_REPEAT_INFINITE;
	else 
		flag = 0;
    m_pDirectMusicSegment->SetRepeats(flag);

    m_pDirectMusicPerformance->PlaySegmentEx(m_pDirectMusicSegment, NULL, NULL, 0, 0, NULL, NULL, NULL);
	m_bStop = false;
}

void CMusicMidi::Play()
{
	if(m_pDirectMusicPerformance->IsPlaying(m_pDirectMusicSegment, NULL) != S_OK)
	{
		m_pDirectMusicSegment->SetStartPoint(m_time);    //���ϴ���ͣ��λ�ü�������
		m_pDirectMusicPerformance->PlaySegmentEx(m_pDirectMusicSegment, NULL, NULL, 0, 0, NULL, NULL, NULL);
		m_bStop = false;
	}
}
void CMusicMidi::Stop()
{
	if(m_pDirectMusicPerformance->IsPlaying(m_pDirectMusicSegment, NULL) == S_OK)	//���ڲ���
	{	
		m_pDirectMusicPerformance->StopEx(m_pDirectMusicSegment, 0, DMUS_SEGF_DEFAULT);
		m_pDirectMusicPerformance->GetTime(NULL, &m_time);	
		m_bStop = true;
	}
}

void CMusicMidi::ShutDown()
{
	if(m_pDirectMusicLoader)
	{
		m_pDirectMusicLoader->ReleaseObjectByUnknown(m_pDirectMusicSegment);
		m_pDirectMusicLoader->ClearCache(GUID_DirectMusicAllTypes);
		m_pDirectMusicLoader->Release();
		m_pDirectMusicLoader = NULL; 
	}
	if(m_pDirectMusicSegment)
	{
		m_pDirectMusicSegment->Unload(m_pDirectMusicPerformance);
		m_pDirectMusicSegment->Release();
		m_pDirectMusicSegment = NULL; 
	}
	if(m_pDirectMusicPerformance)
	{
		m_pDirectMusicPerformance->Release();
		m_pDirectMusicPerformance = NULL; 
	}
	CoUninitialize();

}
void CMusicMidi::SetVolume(long volume)
{
	//m_pDirectMusicPerformance->SetParam(GUID_PerfMasterVolume, sizeof(long), 0, 0, &m_volume );
}
