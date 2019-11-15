/*
 * @Description: 录音或播放
 * @Author: your name
 * @Date: 2019-07-20 10:53:25
 * @LastEditTime: 2019-07-21 23:41:07
 * @LastEditors: Please set LastEditors
 */

#include "voice.h"
#include "alsa/asoundlib.h"

/**
 * @description: 设置硬件参数
 * @param {type} 
 * @return: 
 */
int set_pcm(pcm_container *sound, wav_format *wav) {
    int ret = -1;
    
    //分配snd_pcm_hw_params_t结构体空间
    snd_pcm_hw_params *params;
    ret = snd_pcm_hw_params_malloc(&params);
    // snd_pcm_hw_params_alloca(params); 
    if(ret < 0){
        return ret;
    }

    //初始化params
    ret = snd_pcm_hw_params_any(sound->handle, params);
    if (ret < 0){
        printf("%s\n", snd_strerror(ret));
    }

    //初始化访问模式（交错模式）
    snd_pcm_hw_params_set_access(sund->handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);    
    
    //初始化量化参数
    switch(bit/8){
        case 1:snd_pcm_hw_params_set_format(sound->handle, params, SND_PCM_FORMAT_U8);    break ;
        case 2:snd_pcm_hw_params_set_format(sound->handle, params, SND_PCM_FORMAT_S16_LE);break ;
        case 3:snd_pcm_hw_params_set_format(sound->handle, params, SND_PCM_FORMAT_S24_LE);break ;
        default:snd_pcm_hw_params_set_format(sound->handle, params, SND_PCM_FORMAT_S16_LE);break ;
    }

    //设置通道数量
    snd_pcm_hw_params_set_channels(sound->handle, params, wav->channels);  //设置声道,1表示单声>道，2表示立体声

    //设置采样率，如果硬件不支持我们设置的采样率，将使用最接近的,有些录音采样频率固定为8KHz
    unsigned int value = wav->format.sample_rate;
    snd_pcm_hw_params_set_rate_near(sound->handle, params, &value, 0);

    //设置周期（以帧为单位）帧=声道数×量化级别
    snd_pcm_hw_params_set_buffer_size_near(sound->handle, params, &sound->frrams_per_buffer);

    //设置周期（以帧为单位）帧=声道数×量化级别
    snd_pcm_hw_params_set_period_size_near(sound->handle, params, &sound->frams_per_period, 0);

    //设置hw_params
    snd_pcm_hw_params(sound->handle, params);
   
    sound->bits_per_sample = snd_pcm_format_physical_width(pcm_format);
    sound->bytes_per_sample = sound->bits_per_sample / 8 * wav->format.channels;

    sound->period_buff = (unsigned int *)calloc(1, sound->frames_per_period * sound->bytes_per_frame);
}

/**
 * @description: 设置wav文件头部信息
 * @param {type} 
 * @return: 
 */
int set_wav_header_info(const int fd, wav_format * const wav) {
    //设置WAV格式参数
    wav->head.id = RIFF;
    wav->head.format = WAVE;
    wav->head.size = LE_INT(36+wav->data.data_size());

    wav->format.fmt_id = FMT;
    wav->format.fmt_size = LE_INT(16);
    wav->format.fmt = LE_SHORT(WAV_FMT_PCM);
    wav->format.channels = LE_SHORT(2);
    wav->format.sample_rate = LE_INT(44100);   //采样频率
    wav->format.bits_per_sample = LE_SHORT(16);//量化位数
    wav->format.block_align = LE_SHORT(wav->format.channels * wav->format.bits_per_sample/8);
    wav->format.byte_rate = LE_INT(wav->format.sample_rate * wav->format.block_align)；//码率，每秒通过的字节数
    
    wav->data.id = DATA;
    wav->data.data_size = LE_INT(1000* wav->format.byte_rate);

    return fd;
}

/**
 * @description: 获取wav文件头部信息
 * @param {type} 
 * @return: 
 */
int get_wav_header_info(const int fd, wav_format * const wav);  {
    int n1 = read(fd, &wav->head, sizeof(wav->head));
    int n2 = read(fd, &wav->format, sizeof(wav->format));
    int n3 = read(fd, &wav->data, sizeof(wav->data));
    
    if(n1 != sizeof(wav->head) || n2 != sizeof(wav->format) || n3 != sizeof(wav->data)) {
        return -1;
    }
    return fd;
    //int n = read(fd, &wav, sizeof(wav));
}


//     snd_pcm_uframes_t read_pcm_data(pcm_container *sound, snd_pcmuframs_t frrams) {
//     snd_pcm_uframes_t exact_frames = 0;
//     snd_pcm_uframes_t n=0;

//     uint8_t *p = sound->period_buf;
//     while(frams > 0) {
//         n = snd_pcm_read(sound->hanfle, p, frams);
//         frams -= n;
//         exact_frams +=n;
//         p += n* sound->bytes_per_frams;
//     }
//     return exact_frames;
// }

/**
 * @description: 从PCM设备录制音频
 * @param {type} 
 * @return: 
 */
void read_pcm(const int fd, pcm_container * const sound, wav_format * const wav) {
    //写入文件头
    write(fd, &wav->head, sizeof(wav->head));
    write(fd, &wav->format, sizeof(wav->format));
    write(fd, &wav->data, sizeof(wav->data));

    //写入数据
    unsigned int total_byte = wav->data.data.size;
    unsigned int w = 0;

    while(total_byte > 0) {
        unsigned int total_frame = total_byte / (sound->bytes_per_frame);
        snd_pcm_uframes_t n = MIN(total_frams, sound->frames_per_period);

        unsigned int frames_read = read_pcm_data(sound, n);
        unsigned int w = write(fd, sound->period_buff, frames_read * sound->bytes_per_frame);
        total_byte -=(frames_read * sound->bytes_per_frame);
    } 
}


/**
 * @description: 读wav文件
 * @param {type} 
 * @return: 
 */
void read_file(const int fd, void *buff, size_t count) {
    ssize_t result = 0, res;
    while (count > 0) {
        if((res == read(fd, buf, count)) == 0){
            break;
        }
        if(res < 0) {
            return result > 0 ? result : res;
        }
        count -=res;
        result += res;
        buff = (char *) buff + res;
    }
    return result;
}

/**
 * @description: 写入pcm设备
 * @param {type} 
 * @return: 
 */
void write_pcm(const int fd, pcm_container * const sound, wav_format * const wav) {
    
    int load = 0,ret=0;
    off64_t writeLen = 0;
    off64_t c;
    off64_t total_bytes = LE_INT(wav->data.data_size);
    uint32_t period_bytes = pcm->frames_per_period * pcm->bytes_per_frame;

    while (writeLen < total_bytes) {
        do {
            c = total_bytes - writeLen;
            if(c > period_bytes) {
                c = period_bytes;
            }
            c -=load;

            if(0 == c) {
                break;
            }

            ret = read_file();

            if (0 == ret) {
                break;
            }
            load +=ret;
        } while((size_t)load < period_bytes);

        load = load / pcm->bytes_per_frame;

         //9. 写音频数据到PCM设备
        ret = snd_pcm_writei(handle, buffer, frames)) < 0);
        if(ret != load ){
            switch (ret) {
                case -EPIPE:
                    /* EPIPE means underrun */
                    fprintf(stderr, "underrun occurred\n");
                    //完成硬件参数设置，使设备准备好
                    snd_pcm_prepare(handle);
                    break;
                case -EBADFD:    //错误
                case -ESTRPIPE:  //挂起
                    fprintf(stderr,"error from writei: %s\n",snd_strerror(ret));
                    if(playFlag == true){
                        break;
                    }
                    break;
                default:
                    printf("未知错误:%d\n",ret);
                    break;
            }
            break;
        }

        ret = ret * sound->bytes_per_frame;
        writelen +=ret;
        load = 0;
    }
}


/**
 * @description: PCM设备
 * @param {type} 
 * @return: 
 */
void play(int fd) {
    //SND_PCM_STREAM_PLAYBACK,
    //以回放方式打开pcm设备，
    //打开PCM设备，最后一个参数为0意味着标准配置
    //
// plughw:0,0
    pcm_container *sound = calloc(1, sizeof(pcm_container));
    int ret = snd_pcm_open(&sound->handle, "default", SND_PCM_STREAM_CAPTURE, 0);
    if(ret < 0){
        return;
    }

    //分配结构体空间
    wav_format *wav = calloc(1, sizeof(wav_format));

    //wav文件格式参数设置或获取
    set_wav_header_info(wav);
    get_wav_header_info(wav);
    
    //根据从wav文件的头部信息，设置pcm设备参数
    set_pcm(sound, wav);

    //录音或播放
    read_pcm(fd, playback, wav);
    write_pcm(fd, playback, wav);

    //释放资源
    snd_pcm_drain(sound->handle);
    snd_pcm_close(sound->handle);
    close(fd);

    free(sound->period_buf);
    free(sound);
    free(wav);
}

/**
 * @description: 录音
 * @param {type} 
 * @return: 
 */
void recoder_voice(char *fileName) {
    if(NULL == fileName){
        return -1;
    }

    //打开wav文件
    int fd = open(fileName, O_CREAT|O_WEONLY|O_TRUNC, 0777);
    if(fd < 0){
        return fd;
    }

    play(fd);
}


/**
 * @description: 播放音频
 * @param {type} 
 * @return: 
 */
void play_voice(char *fileName) {
    if(NULL == fileName){
        return -1;
    }

    //打开wav文件，获取音频信息
    int fd = open(fileName, O_RDONLY);
    if(fd < 0){
        return fd;
    }

    play(fd);
}



/**
 * @description: wav格式
 * 分为三段：RIFF段（格式ID、大小），FMT段（音频信息：采样频率等），DATA（音频数据）
 */
// struct WAV_HEADER
// {
//     char rld[4];            //riff 标志符号
//     int rLen;
//     char wld[4];            //格式类型（wave）
//     char fld[4];            //"fmt"

//     int fLen;               //sizeof(wave format matex)
//     short wFormatTag;       //编码格式
//     short wChannels;        //声道数
//     int   nSamplesPersec ;  //采样频率
//     int   nAvgBitsPerSample;//WAVE文件采样大小
//     short wBlockAlign;      //块对齐
//     short wBitsPerSample;   //WAVE文件采样大小

//     char  dld[4];           //”data“
//     int   wSampleLength;    //音频数据的大小

// } wav_header;




//# include <stdio.h>
//# include <unistd.h>
//# include <sys/stat.h>
//# include <sys/mman.h>
//# include <sys/fcntl.h>
//# include "mad.h"
//# include "alsa/asoundlib.h"

//static int decode(unsigned char const *, unsigned long);
//int set_pcm_mp3(snd_pcm_t* handle,snd_pcm_hw_params_t* params);
//void set_volume(long volume);

//snd_pcm_t* handle=NULL;          //PCI设备句柄
//snd_pcm_hw_params_t* params=NULL;//硬件信息和PCM流配置

//int play_mp3(char *fileName)
//{

//    long volume=100;

//    struct stat stat;
//    void *fdm;

//    int fd=open(fileName,O_RDWR);
//    if(fd<0){
//        perror("open file failed:");
//        return 1;
//    }
//    //fstat()用来将fd所指的文件状态，复制到参数buf所指的结构中(struct stat)
//    if (fstat(fd, &stat) == -1 || stat.st_size == 0){
//        printf("fstat failed:\n");
//        return 2;
//    }
//    printf("size:%ld字节--->%.2fM\n",stat.st_size,(float)stat.st_size/1024/1024);

//    fdm = mmap(0, stat.st_size, PROT_READ, MAP_SHARED, fd, 0);
//    if (fdm == MAP_FAILED){
//        return 3;
//    }

//    //设置pcm 参数
//    if(set_pcm_mp3(handle,params) != 0) {
//        printf("set_pcm fialed:\n");
//        return 1;
//    }
//    set_volume(volume);
//    decode((unsigned char *)fdm, stat.st_size);

//    if (munmap(fdm, stat.st_size) == -1){
//        return 4;
//    }
//    snd_pcm_drain(handle);
//    snd_pcm_close(handle);
//    return 0;
//}


////alsa设置相关函数
//int set_pcm_mp3(snd_pcm_t* handle,snd_pcm_hw_params_t* params)
//{
//    int rc;
//    int dir=0;
//    unsigned int rate = 44100;          /* 采样频率 44.1KHz*/
//    int format = SND_PCM_FORMAT_S16_LE; /* 量化位数 16 */
//    int channels = 2;                   /* 声道数 2 */

//    rc=snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
//    if(rc<0)    {
//      perror("\nopen PCM device failed:");
//      exit(1);
//    }
//    snd_pcm_hw_params_alloca(&params); //分配params结构体

//    rc=snd_pcm_hw_params_any(handle, params);//初始化params
//    if(rc<0){
//      perror("\nsnd_pcm_hw_params_any:");
//      exit(1);
//    }
//    rc=snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED); //初始化访问权限
//    if(rc<0){
//      perror("\nsed_pcm_hw_set_access:");
//      exit(1);
//    }
//    rc=snd_pcm_hw_params_set_format(handle, params, (enum _snd_pcm_format)format); //设置16位采样精度
//    if(rc<0){
//      perror("snd_pcm_hw_params_set_format failed:");
//      exit(1);
//    }
//    rc=snd_pcm_hw_params_set_channels(handle, params, channels); //设置声道,1表示单声>道，2表示立体声
//    if(rc<0){
//      perror("\nsnd_pcm_hw_params_set_channels:");
//      exit(1);
//    }
//    rc=snd_pcm_hw_params_set_rate_near(handle, params, &rate, &dir); //设置>频率
//    if(rc<0){
//      perror("\nsnd_pcm_hw_params_set_rate_near:");
//      exit(1);
//    }
//    rc = snd_pcm_hw_params(handle, params);
//    if(rc<0)    {
//      perror("\nsnd_pcm_hw_params: ");
//      exit(1);
//    }
//    return 0;
//}


//void set_volume(long volume)
//{
//    snd_mixer_t *mixerFd;
//    snd_mixer_elem_t *elem;
//    long minVolume = 0,maxVolume = 100;
//    int result;
//    // 打开混音器
//    if ((result = snd_mixer_open( &mixerFd, 0)) < 0){
//        printf("snd_mixer_open error!\n");
//        mixerFd = NULL;
//    }
//    // Attach an HCTL to an opened mixer
//    if ((result = snd_mixer_attach( mixerFd, "default")) < 0){
//        printf("snd_mixer_attach error!\n");
//        snd_mixer_close(mixerFd);
//        mixerFd = NULL;
//    }
//    // 注册混音器
//    if ((result = snd_mixer_selem_register( mixerFd, NULL, NULL)) < 0){
//        printf("snd_mixer_selem_register error!\n");
//        snd_mixer_close(mixerFd);
//        mixerFd = NULL;
//    }
//    // 加载混音器
//    if ((result = snd_mixer_load( mixerFd)) < 0){
//        printf("snd_mixer_load error!\n");
//        snd_mixer_close(mixerFd);
//        mixerFd = NULL;
//    }

//    // 遍历混音器元素
//    for(elem=snd_mixer_first_elem(mixerFd); elem; elem=snd_mixer_elem_next(elem)){
//        if (snd_mixer_elem_get_type(elem) == SND_MIXER_ELEM_SIMPLE &&
//            snd_mixer_selem_is_active(elem)){ // 找到可以用的, 激活的elem
//            snd_mixer_selem_get_playback_volume_range(elem, &minVolume, &maxVolume);
//            snd_mixer_selem_set_playback_volume_all(elem, volume);
//        }
//    }
//    snd_mixer_close(mixerFd);
//}

////libmad设置相关函数
///*
// * This is a private message structure. A generic pointer to this structure
// * is passed to each of the callback functions. Put here any data you need
// * to access from within the callbacks.
// */
//struct buffer {
//  unsigned char const *start;
//  unsigned long length;
//};

///*
// * This is the input callback. The purpose of this callback is to (re)fill
// * the stream buffer which is to be decoded. In this example, an entire file
// * has been mapped into memory, so we just call mad_stream_buffer() with the
// * address and length of the mapping. When this callback is called a second
// * time, we are finished decoding.
// */

//static enum mad_flow input(void *data,struct mad_stream *stream)
//{
//  struct buffer *buffer = (struct buffer *)data;
//  if (!buffer->length)
//    return MAD_FLOW_STOP;

//  mad_stream_buffer(stream, buffer->start, buffer->length);
//  buffer->length = 0;
//  return MAD_FLOW_CONTINUE;
//}

///*
// * The following utility routine performs simple rounding, clipping, and
// * scaling of MAD's high-resolution samples down to 16 bits. It does not
// * perform any dithering or noise shaping, which would be recommended to
// * obtain any exceptional audio quality. It is therefore not recommended to
// * use this routine if high-quality output is desired.
// */

//static inline signed int scale(mad_fixed_t sample)
//{
//  /* round */
//  sample += (1L << (MAD_F_FRACBITS - 16));

//  /* clip */
//  if (sample >= MAD_F_ONE)
//    sample = MAD_F_ONE - 1;
//  else if (sample < -MAD_F_ONE)
//    sample = -MAD_F_ONE;

//  /* quantize */
//  return sample >> (MAD_F_FRACBITS + 1 - 16);
//}

///*
// * This is the output callback function. It is called after each frame of
// * MPEG audio data has been completely decoded. The purpose of this callback
// * is to output (or play) the decoded PCM audio.
// */

//static enum mad_flow output(void *data,struct mad_header const *header,struct mad_pcm *pcm)
//{
//    unsigned int nchannels, nsamples,n;
//    mad_fixed_t const *left_ch, *right_ch;

//    /* pcm->samplerate contains the sampling frequency */
//    nchannels = pcm->channels;
//    n=nsamples = pcm->length;
//    left_ch = pcm->samples[0];
//    right_ch = pcm->samples[1];

//    unsigned char Output[6912], *OutputPtr;
//    int fmt, wrote, speed, exact_rate, err, dir;

//    OutputPtr = Output;

//    while (nsamples--){
//        signed int sample;

//        /* output sample(s) in 16-bit signed little-endian PCM */
//        sample = scale(*left_ch++);

//        *(OutputPtr++) = sample >> 0;
//        *(OutputPtr++) = sample >> 8;
//        if (nchannels == 2){
//            sample = scale (*right_ch++);
//            *(OutputPtr++) = sample >> 0;
//            *(OutputPtr++) = sample >> 8;
//        }
//    }
//    OutputPtr = Output;
//    snd_pcm_writei (handle, OutputPtr, n);
//    OutputPtr = Output;

//    return MAD_FLOW_CONTINUE;
//}

///*
// * This is the error callback function. It is called whenever a decoding
// * error occurs. The error is indicated by stream->error; the list of
// * possible MAD_ERROR_* errors can be found in the mad.h (or stream.h)
// * header file.
// */

//static enum mad_flow error(void *data,struct mad_stream *stream,struct mad_frame *frame)
//{
//    struct buffer *buffer = (struct buffer *)data;
//    printf("this is mad_flow error\n");
//    fprintf(stderr, "decoding error 0x%04x (%s) at byte offset %u\n",
//    stream->error, mad_stream_errorstr(stream),
//    stream->this_frame - buffer->start);

//    /* return MAD_FLOW_BREAK here to stop decoding (and propagate an error) */
//    return MAD_FLOW_CONTINUE;
//}

///*
// * This is the function called by main() above to perform all the decoding.
// * It instantiates a decoder object and configures it with the input,
// * output, and error callback functions above. A single call to
// * mad_decoder_run() continues until a callback function returns
// * MAD_FLOW_STOP (to stop decoding) or MAD_FLOW_BREAK (to stop decoding and
// * signal an error).
// */
//static int decode(unsigned char const *start, unsigned long length){
//  struct buffer buffer;
//  struct mad_decoder decoder;
//  int result;

//  /* initialize our private message structure */
//  buffer.start = start;
//  buffer.length = length;

//  /* configure input, output, and error functions */
//  mad_decoder_init(&decoder, &buffer,
//           input, 0 /* header */, 0 /* filter */, output,
//           error, 0 /* message */);

//  /* start decoding */
//  result = mad_decoder_run(&decoder, MAD_DECODER_MODE_SYNC);

//  /* release the decoder */
//  mad_decoder_finish(&decoder);

//  return result;
//}

////home/yk/Desktop/MP3/mp3/playmp3.cpp:416: error: undefined reference to `mad_stream_buffer'





