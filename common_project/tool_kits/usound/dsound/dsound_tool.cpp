#include <atlbase.h>
#include <WTypes.h>
#include <shellapi.h>
#include "dsound_tool.h"
#include "wave/WaveFile.h"
#include "common\tool/common_tool.h"
#include "base/util/string_util.h"
#include "MediaDevice.h"

#pragma comment(lib,"winmm.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib,"dxguid.lib")
void DSoundTool::SetSystemAudio()
{
	float main_volumn = 0;
	GetSystemVolume(main_volumn);
	if (main_volumn < 20)
	{
		SetSystemVolume(35);
	}

	DWORD mic_volumn = 0;
	GetMicrophoneValue(mic_volumn);
	if (mic_volumn < 30)
	{
		SetMicrophoneValue(55);
	}

	float min, max, step;
	GetMicroBoostData(min, max, step);
	float boost;
	GetMicroBoostLevel(boost);
	if (boost < 10)
	{
		SetMicroBoostLevel(20);
	}

}
BOOL CALLBACK DSoundTool::enum_dev_proc(LPGUID lpGUID, LPCTSTR lpszDesc,
	LPCTSTR lpszDrvName, LPVOID lpContext)
{
	
	wprintf(L"%s-- %s\n", lpszDesc, lpszDrvName);
	if (lpGUID)
	{
		std::vector<DsoundDevice>* list = (std::vector<DsoundDevice>*)(lpContext);
		DsoundDevice device = {lpGUID, lpszDesc};
		list->push_back(device);
	
	}

	return(TRUE);
}

BOOL DSoundTool::EnumDevices(std::vector<DsoundDevice>&vec, bool b_capture)
{
	return  b_capture ? FAILED(DirectSoundCaptureEnumerate(
		(LPDSENUMCALLBACK)(DSoundTool::enum_dev_proc),
		(VOID*)&vec)) : FAILED(DirectSoundEnumerate(
		(LPDSENUMCALLBACK)(DSoundTool::enum_dev_proc),
		(VOID*)&vec));
	
}
void DSoundTool::InitSpeaker()
{
	vec_device.clear();
	EnumDevices(vec_device, false);
}
DsoundDevice* DSoundTool::GetSpeakerByName(std::wstring device_name)
{
	for (size_t i = 0; i < vec_device.size(); i++)
	{

		if (vec_device[i].device_name_.find(device_name) == 0)
			return &vec_device[i];
	}
	return NULL;
}

std::vector<DsoundDevice>& DSoundTool::GetSpeakers()
{
	return vec_device;
}
void DSoundTool::PrintMicProps(HWND hwnd)
{
	LPDIRECTSOUND8 lpds;
	HRESULT hr = DirectSoundCreate8(NULL, &lpds, NULL);
	hr = lpds->SetCooperativeLevel(hwnd, DSSCL_NORMAL);
	if (FAILED(hr))
	{
		
	}
	
	//IDirectSoundBuffer8::Restore
	//IDirectSoundCaptureBuffer8::AddRef

}

bool DSoundTool::StartRecord()
{
	HRESULT hr;
	
	//m_guidCapDevId = dev_id;
	//ZeroMemory(&m_aPosNotify, sizeof(DSBPOSITIONNOTIFY)* (NUM_REC_NOTIFICATIONS + 1));
	//m_dwCapBufSize = 0;
	//m_dwNotifySize = 0;
	// Create IDirectSoundCapture using the preferred capture device
	//hr = DirectSoundCaptureCreate(NULL, &m_pCapDev, NULL);

	//DSCBUFFERDESC               dscbd;
	//LPDIRECTSOUNDCAPTUREBUFFER  pDSCB;
	//WAVEFORMATEX                wfx =
	//{ WAVE_FORMAT_PCM, 2, 44100, 176400, 4, 16, 0 };
	//// wFormatTag, nChannels, nSamplesPerSec, mAvgBytesPerSec,
	//// nBlockAlign, wBitsPerSample, cbSize

	////LPDIRECTSOUNDCAPTUREBUFFER8 ppDSCB8;
	////if ((NULL == pDSC) || (NULL == ppDSCB8)) return E_INVALIDARG;
	//dscbd.dwSize = sizeof(DSCBUFFERDESC);
	//dscbd.dwFlags = 0;
	//dscbd.dwBufferBytes = 10*wfx.nAvgBytesPerSec;
	//dscbd.dwReserved = 0;
	//dscbd.lpwfxFormat = &wfx;
	//dscbd.dwFXCount = 0;
	//dscbd.lpDSCFXDesc = NULL;

	//if (SUCCEEDED(hr = m_pCapDev->CreateCaptureBuffer(&dscbd, &pDSCB, NULL)))
	//{
	//	//hr = pDSCB->QueryInterface(IID_IDirectSoundCaptureBuffer8, (LPVOID*)&ppDSCB8);
	//	pDSCB->Release();
	//}

	////ppDSCB8->Start(DSCBSTART_LOOPING);
	//return SUCCEEDED(hr);
	return 1;
}

bool DSoundTool::StopPlaying()
{
	if (m_pDSBuffer8)
	{
		m_pDSBuffer8->Stop();
	}
	
	SAFE_RELEASE(m_pDSBuffer8);
	SAFE_RELEASE(m_pDS);
	return true;
}

bool DSoundTool::PlayWav(HWND hwnd, std::wstring device_name_)
{
	StopPlaying();
	InitSpeaker();
	//int sample_rate = 44100;  //PCM sample rate  
	int channels = 1;         //PCM channel number  
	int bits_per_sample = 16; //bits per sample  
	int sample_rate = 8000;

	//IDirectSound8 *m_pDS = NULL;
	//IDirectSoundBuffer8 *m_pDSBuffer8 = NULL; //used to manage sound buffers.  
	IDirectSoundBuffer *m_pDSBuffer = NULL;
	//DSBPOSITIONNOTIFY m_pDSPosNotify;
	//IDirectSoundNotify8 *m_pDSNotify = NULL;

	DsoundDevice* device = GetSpeakerByName(device_name_);
	LPGUID guid = NULL;
	if (device != NULL)
	{
		guid = device->guid_;
	}
	//SetConsoleTitle(TEXT("Simplest Audio Play DirectSound"));//Console Title  
	//Init DirectSound  
	if (FAILED(DirectSoundCreate8(guid, &m_pDS, NULL)))
		return FALSE;
	if (FAILED(m_pDS->SetCooperativeLevel(hwnd, DSSCL_NORMAL)))
	{
		SAFE_RELEASE(m_pDS);
		return FALSE;
	}		

	WAVEFORMATEX wfe;
	wfe.cbSize = 0;
	wfe.wFormatTag = WAVE_FORMAT_PCM;
	wfe.nChannels = channels;
	wfe.nSamplesPerSec = sample_rate;
	wfe.nAvgBytesPerSec = sample_rate*(bits_per_sample / 8)*channels;
	wfe.nBlockAlign = (bits_per_sample / 8)*channels;
	wfe.wBitsPerSample = bits_per_sample;

	CWaveFile wave_file;
	std::wstring wav_path = commontool::GetAppPath().append(L"res\\audio\\soundTest.wav");
	wave_file.Open((LPTSTR)(wav_path.c_str()), &wfe, WAVEFILE_READ);


	DSBUFFERDESC dsbd;
	memset(&dsbd, 0, sizeof(dsbd));
	dsbd.dwSize = sizeof(dsbd);
	dsbd.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GETCURRENTPOSITION2;
	dsbd.dwBufferBytes = wave_file.GetSize();
	//WAVE Header  
	dsbd.lpwfxFormat = &wfe;

	//Creates a sound buffer object to manage audio samples.   
	HRESULT hr1;
	if (FAILED(m_pDS->CreateSoundBuffer(&dsbd, &m_pDSBuffer, NULL))){
		SAFE_RELEASE(m_pDS);
		return FALSE;
	}
	if (FAILED(m_pDSBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&m_pDSBuffer8))){
		SAFE_RELEASE(m_pDS);
		SAFE_RELEASE(m_pDSBuffer);
		return FALSE;
	}

	SAFE_RELEASE(m_pDSBuffer);

	//Start Playing  
	LPVOID buf = NULL;
	DWORD  buf_len = 0;
	DWORD	read;


	m_pDSBuffer8->Lock(0, 0, &buf, &buf_len, NULL, NULL, DSBLOCK_ENTIREBUFFER);
	//memcpy(buf, p_pcm_data, buf_len);
	wave_file.Read((BYTE*)buf, buf_len, &read);
	wave_file.Close();
	m_pDSBuffer8->Unlock(buf, buf_len, NULL, 0);

	m_pDSBuffer8->SetCurrentPosition(0);
	m_pDSBuffer8->Play(0, 0, 0);

	//WaitForMultipleObjects(1, &m_event, FALSE, INFINITE);
	//SAFE_RELEASE(m_pDSBuffer8);
	
	//SAFE_RELEASE(m_pDS);
	return true;
	
}
#define MAX_AUDIO_BUF 4   
#define BUFFERNOTIFYSIZE 640 //192000 
bool DSoundTool::PlayPCM2(HWND hwnd, const void* pcm_data, int data_len)
{
	int i;
	/*FILE * fp;
	if ((fp = fopen("send_pcm16.pcm", "rb")) == NULL){
		printf("cannot open this file\n");
		return -1;
	}*/
	int sample_rate = 16000;  //PCM sample rate  
	int channels = 1;         //PCM channel number  
	int bits_per_sample = 16; //bits per sample  

	IDirectSound8 *m_pDS = NULL;
	IDirectSoundBuffer8 *m_pDSBuffer8 = NULL;	//used to manage sound buffers.
	IDirectSoundBuffer *m_pDSBuffer = NULL;
	IDirectSoundNotify8 *m_pDSNotify = NULL;
	DSBPOSITIONNOTIFY m_pDSPosNotify[MAX_AUDIO_BUF];
	HANDLE m_event[MAX_AUDIO_BUF];

	SetConsoleTitle(TEXT("Simplest Audio Play DirectSound"));//Console Title
	//Init DirectSound
	if (FAILED(DirectSoundCreate8(NULL, &m_pDS, NULL)))
		return FALSE;
	if (FAILED(m_pDS->SetCooperativeLevel(hwnd, DSSCL_NORMAL)))
		return FALSE;


	DSBUFFERDESC dsbd;
	memset(&dsbd, 0, sizeof(dsbd));
	dsbd.dwSize = sizeof(dsbd);
	dsbd.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GETCURRENTPOSITION2;
	dsbd.dwBufferBytes = MAX_AUDIO_BUF*BUFFERNOTIFYSIZE;
	//WAVE Header
	dsbd.lpwfxFormat = (WAVEFORMATEX*)malloc(sizeof(WAVEFORMATEX));
	dsbd.lpwfxFormat->wFormatTag = WAVE_FORMAT_PCM;
	/* format type */
	(dsbd.lpwfxFormat)->nChannels = channels;
	/* number of channels (i.e. mono, stereo...) */
	(dsbd.lpwfxFormat)->nSamplesPerSec = sample_rate;
	/* sample rate */
	(dsbd.lpwfxFormat)->nAvgBytesPerSec = sample_rate*(bits_per_sample / 8)*channels;
	/* for buffer estimation */
	(dsbd.lpwfxFormat)->nBlockAlign = (bits_per_sample / 8)*channels;
	/* block size of data */
	(dsbd.lpwfxFormat)->wBitsPerSample = bits_per_sample;
	/* number of bits per sample of mono data */
	(dsbd.lpwfxFormat)->cbSize = 0;

	//Creates a sound buffer object to manage audio samples. 
	HRESULT hr1;
	if (FAILED(m_pDS->CreateSoundBuffer(&dsbd, &m_pDSBuffer, NULL))){
		return FALSE;
	}
	if (FAILED(m_pDSBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&m_pDSBuffer8))){
		return FALSE;
	}
	//Get IDirectSoundNotify8
	if (FAILED(m_pDSBuffer8->QueryInterface(IID_IDirectSoundNotify, (LPVOID*)&m_pDSNotify))){
		return FALSE;
	}
	for (i = 0; i<MAX_AUDIO_BUF; i++){
		m_pDSPosNotify[i].dwOffset = i*BUFFERNOTIFYSIZE;
		m_event[i] = ::CreateEvent(NULL, false, false, NULL);
		m_pDSPosNotify[i].hEventNotify = m_event[i];
	}
	m_pDSNotify->SetNotificationPositions(MAX_AUDIO_BUF, m_pDSPosNotify);
	m_pDSNotify->Release();

	//Start Playing
	BOOL isPlaying = TRUE;
	LPVOID buf = NULL;
	DWORD  buf_len = 0;
	DWORD res = WAIT_OBJECT_0;
	DWORD offset = BUFFERNOTIFYSIZE;

	m_pDSBuffer8->SetCurrentPosition(0);
	m_pDSBuffer8->Play(0, 0, 0);
	//Loop
	//while (isPlaying){
		if ((res >= WAIT_OBJECT_0) && (res <= WAIT_OBJECT_0 + 3)){
			m_pDSBuffer8->Lock(offset, BUFFERNOTIFYSIZE, &buf, &buf_len, NULL, NULL, 0);
			//if (fread(buf, 1, buf_len, fp) != buf_len){
			//	//File End
			//	//Loop:
			//	fseek(fp, 0, SEEK_SET);
			//	fread(buf, 1, buf_len, fp);
			//	//Close:
			//	//isPlaying=0;
			//}
			char* p = const_cast<char*>((char*)pcm_data);
			memcpy(buf, p, BUFFERNOTIFYSIZE);
			p += BUFFERNOTIFYSIZE;
			m_pDSBuffer8->Unlock(buf, buf_len, NULL, 0);
			offset += buf_len;
			offset %= (BUFFERNOTIFYSIZE * MAX_AUDIO_BUF);
			printf("this is %7d of buffer\n", offset);
		}
		res = WaitForMultipleObjects(MAX_AUDIO_BUF, m_event, FALSE, INFINITE);
	//}

	return 0;
}

void DSoundTool::SetSpeakerVolume(double value)
{
	SetSystemVolume(value);
}