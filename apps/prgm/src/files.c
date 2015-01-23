//files.c
#include "files.h"

#include <stdlib.h>
#include <string.h>

#include "app.h"
#include "delay.h"
#include "filesystem.h" //includes fat_filelib.h
#include "flash.h"
#include "memory.h"
#include "print_funcs.h"

//file directory
typedef struct _dirList {
    char path[MAX_PATH];
    volatile char nameBuf[DIR_LIST_NAME_BUF_SIZE];
    u32 num;
} dirList_t;

static dirList_t sampleList;


//temporary buffer for sample transfer over spi
u8 *bfinSampleData;
u32 bfinSampleOffset;
u32 bfinSampleSize;

//static function declarations
//memory allocation
static void init_sample_paths(void);
static void alloc_sample_buffer(void);
static prgmSampleptr alloc_sample(void);
static void init_sample_param(prgmSample *s, u8 num);

//file handling
static void files_load_sample(u8 idx);
static void fake_fread(volatile u8 *dst, u32 len, void *fp);
static char *sample_filepath(s32 n);
static bool strip_extension(char *str);


//extern function definitions
extern void samples_init(void) {
    u8 i;
    
    //  init variables
    n_samples = 0;
    bfinSampleOffset = 0;
    bfinSampleSize = 0;
    
    //  init array of sample data
    for (i=0; i<N_SAMPLES; i++)
    {
        sample[i] = alloc_sample();
        init_sample_param(sample[i], i);
    }
    print_dbg("\r\n finished alloc_sample() and init_sample_param() ");

    //  set sample data for recording buffers
    sample[0]->offset = 0;
    sample[0]->size = SAMPLE_SIZE;
    n_samples++;
    
    sample[1]->offset = sample[0]->offset + sample[0]->size;
    sample[1]->size = SAMPLE_SIZE;
    n_samples++;
    
    sample[2]->offset = sample[1]->offset + sample[1]->size;
    sample[2]->size = SAMPLE_SIZE;
    n_samples++;
    
    sample[3]->offset = sample[2]->offset + sample[2]->size;
    sample[3]->size = SAMPLE_SIZE;
    n_samples++;
    
    sample[4]->offset = sample[3]->offset + sample[3]->size;
    
    //  init wav file paths and sample loading buffer
    init_sample_paths();
    print_dbg("\r\n finished init_sample_paths() ");
    alloc_sample_buffer();
    print_dbg("\r\n finished alloc_sample_buffer() ");
}


//static function definitions
void alloc_sample_buffer(void) {
    u32 i;
    
    //  allocate memory
    bfinSampleData = (u8*)alloc_mem(SAMPLE_SIZE * sizeof(u8));
    
    //  zero data
    for (i=0; i<SAMPLE_SIZE; i++) bfinSampleData[i] = 0;
}

prgmSampleptr alloc_sample(void) {
    return(prgmSampleptr)alloc_mem(sizeof(prgmSample));
}

void init_sample_param(prgmSample *s, u8 num) {
    s->num = num;
    s->offset = 0;
    s->size = 0;
}

void init_sample_paths(void) {
    FL_DIR dirstat;
    struct fs_dir_ent dirent; //see fat_access.h
    
    s32 i = 0;
    sampleList.num = 0;
    
    strcpy(sampleList.path, WAV_PATH);
    
    if (fl_opendir(sampleList.path, &dirstat))
    {
        while (fl_readdir(&dirstat, &dirent) == 0)
        {
            if (!(dirent.is_dir))
            {
                sample_path[i] = (char*)alloc_mem(MAX_PATH * (sizeof(char*)));
                sample_name[i] = (char*)alloc_mem(MAX_NAME * (sizeof(char*)));
                strcpy(sampleList.path, WAV_PATH);
                strcat(sampleList.path, dirent.filename);
                //  copy name for on-screen sample select and remove extension
                strcpy(sample_name[i], dirent.filename);
                strip_extension(sample_name[i]);
                //  copy file path
                strcpy(sample_path[i], sampleList.path);
                print_dbg("\r\n sample_path[i]: ");
                print_dbg(sample_path[i]);
                sampleList.num++;

                i++;
                print_dbg("\r\n i: ");
                print_dbg_ulong(i);

                n_samples++;
                print_dbg("\r\n n_samples: ");
                print_dbg_ulong(n_samples);
            }
        }
        //fl_closedir(&dirstat);
    }
}

char *sample_filepath(s32 n) {
    return (n < 0 || n > n_samples) ? sample_path[0] : sample_path[n];
}

void files_load_sample(u8 n) {
    //  pointer to file, size
    void *fp;
    u32 size = 0;
    
    delay_ms(10);
    
    fp = fl_fopen(sample_filepath(n-4), "r");
    print_dbg("\r\n sample_filepath(n) ");
    print_dbg(sample_filepath(n-4));
    
    size = ((FL_FILE*)(fp))->filelength;
    
    if (fp != NULL)
    {
        fake_fread(bfinSampleData, size, fp);
        fl_fclose(fp);
        
        bfinSampleSize = size / sizeof(s32);
        print_dbg("\r\n bfinSampleSize ");
        print_dbg_ulong(bfinSampleSize);
        
        print_dbg("\r\n sample[n-1]->num ");
        print_dbg_ulong(sample[n-1]->num);
        print_dbg("\r\n sample[n-1]->offset ");
        print_dbg_ulong(sample[n-1]->offset);
        print_dbg("\r\n sample[n-1]->size ");
        print_dbg_ulong(sample[n-1]->size);
        
        if (sample[n]->num == n)
        {
            print_dbg("\r\n aligned to sample array! ");
            print_dbg("\r\n sample[n]->num ");
            print_dbg_ulong(sample[n]->num);
            
            print_dbg("\r\n sample[n]->offset ");
            print_dbg_ulong(sample[n]->offset);

            sample[n]->size = bfinSampleSize;
            print_dbg("\r\n sample[n]->size ");
            print_dbg_ulong(sample[n]->size);

            sample[n+1]->offset = sample[n]->offset + sample[n]->size;
            print_dbg("\r\n sample[n+1]->num ");
            print_dbg_ulong(sample[n+1]->num);
            print_dbg("\r\n sample[n+1]->offset ");
            print_dbg_ulong(sample[n+1]->offset);
            print_dbg("\r\n sample[n+1]->size ");
            print_dbg_ulong(sample[n+1]->size);
        }
        else print_dbg("\r\n can not align to sample array!");
    }
    else print_dbg("\r\n file contains no data");
}

void files_load_samples(void) {
    u8 n;
//    static event_t e;
//app_pause();

    for (n=4; n<n_samples; n++)
    {
        //  load sample
        files_load_sample(n);
        
        //  transfer sample
        bfin_fill_buffer(sample[n]->offset, sample[n]->size, (s32*)bfinSampleData);
        print_dbg("\r\n bfin_fill_buffer finished... ");
        
        //  wait for spi process to finish
        delay_ms(50);
 
        //  post event for sample transfer
 //       e.type = kEventAppCustom;
 //       e.data = (s32)i;
 //       event_post(&e);
    }
//app_resume();
}

u8 files_load_dsp(void) {
    void *fp;
    u32 size = 0;
    u8 ret;
    
    delay_ms(10);
    
    app_pause();
    
    fp = fl_fopen(DSP_PATH, "r");
    
    if(fp != NULL) {
        size = ((FL_FILE*)(fp))->filelength;
        fake_fread(bfinLdrData, size, fp);
        
        fl_fclose(fp);
        bfinLdrSize = size;
        
        if(bfinLdrSize > 0) {
            bfin_load_buf(bfinLdrData, bfinLdrSize);
            ret = 1;
        } else {
            print_dbg("\r\n bfin ldr size was <=0, aborting");
            ret = 0;
        }
    } else {
        print_dbg("\r\n error: fp was null in files_load_dsp_name \r\n");
        ret = 0;
    }
    
    app_resume();
    return ret;
}

void fake_fread(volatile u8 *dst, u32 len, void *fp) { //fread: no size arg
    u32 n = 0;
#if 0
    fl_fread(&dst, 1, len, fp);
#else
    while(n < len) {
        *dst = fl_fgetc(fp);
        n++;
        dst++;
    }
#endif
}

bool strip_extension(char *str) {
    int i;
    int dotpos = -1;
    i = strlen(str);
    while(i > 0) {
        --i;
        if(str[i] == '.') {
            dotpos = i;
            break;
        }
    }
    if(dotpos >= 0) {
        str[dotpos] = '\0';
        return 1;
    } else {
        return 0;
    }
}
