#include "hardware.h"

void Hardware::Indicador ()
{
     BITMAP *BMPPointer[2];

     BMPPointer[0] = (BITMAP*) DataP[0].dat;
     BMPPointer[1] = (BITMAP*) DataP[1].dat;

     LocateBuffer (640, 480);

     const int MaxObj = 5144;
     const int Pasos = 10;
     const int AvancePix = BMPPointer[0]->w / Pasos;
     const int AvanceObj = MaxObj / Pasos;

     const int OrigX = 320 - (BMPPointer[0]->w / 2);
     const int OrigY = 240 - BMPPointer[0]->h;
     LogWrite (".");
     if (ObjCargados > MaxObj)
        LogWrite ("\n%i",ObjCargados);

     if (++ObjCargados == MaxObj) { // Si ya terminas esta vez
         RectFill (0, OrigY, 640, OrigY + BMPPointer[0]->h, 0); // Borra la morcilla
         VuelcaBuffer();
         }

     if (!(ObjCargados % AvanceObj)) {
        Dibuja (OrigX, OrigY, (void*) BMPPointer[0]);
        Dibuja ((OrigX - BMPPointer[0]->w) + (ObjCargados / AvanceObj) * AvancePix - 2 * AvancePix,
                OrigY - ((int)(rand() % 30)),
                (void*) BMPPointer[1]);
        RectFill (0, OrigY - 50, 640, OrigY, 0);
        TextOutCentre_aa (SMS->ReadMens (1), OrigY - 50, 255,255,255);
        VuelcaBuffer();
        play_sample((SAMPLE *)DataP[2].dat, 255, 128, 1000, 0);
        }
}

Hardware::Hardware()
{
 LogWrite ("Iniciando Allegro...");
 allegro_init();
 LOCK_FUNCTION(close_button_handler);
 set_close_button_callback(close_button_handler);
 install_keyboard();
 install_timer();
 install_sound (DIGI_AUTODETECT,MIDI_NONE,NULL);
 InitJoy();
 srand(time(0));
 Izquierda = Config->VerTecla (IZQUIERDA);
 Derecha = Config->VerTecla (DERECHA);
 Arriba = Config->VerTecla (ARRIBA);
 Abajo = Config->VerTecla (ABAJO);
 Fuego = Config->VerTecla (FUEGO);
 Selec = Config->VerTecla (SELEC);
 Abort = Config->VerTecla (ABORT);
 ActScreenShot = 0;
 register_png_datafile_object(DAT_PNG); // Para poder usar los PNG's desde archivos .DAT
 char Titulo[128];
 sprintf (Version, "V.%i.%i.%i.%s", VMAJOR, VMINOR, VMICRO, VSTATUS);
 sprintf (Titulo, "Capitan Sevilla, El Remake %s - Computeremuzone Games Studio", Version);
 set_window_title (Titulo);
 ContHD = 0;
 HDDesplC = 0;
 Fuente = NULL;
 OldFont = font;

 set_alpha_blender();

 Mamporrazo = 0;
 MObjetivo = 0;
 Terremot = 0;

 FontCambiado = false;
 Iniciado = false;
 set_color_conversion (COLORCONV_TOTAL);
 for (int n = 0; n < 64; n++)
     for (int i = 0; i < 64; i ++)
          HDDespl[n][i] = 2 * ((rand () % 3) - 1);
 LogWrite ("OK.\n");
 LogWrite ("Iniciando AllegroFont...");
 if (alfont_init())
    LogWrite ("ERROR!!!\n");
  else
    LogWrite ("OK.\n");
 /*LogWrite ("Iniciando FMOD...");
// FSOUND_SetBufferSize(350);
 if (FSOUND_Init(44100, 32, 0))
    LogWrite ("OK.\n");
  else {
    LogWrite ("ERROR (%i): Sonido Inactivo.\n", FSOUND_GetError());
    //Config->ToggleSonido (false);
    }*/
}

Hardware::~Hardware()
{
 LogWrite ("Saliendo de Allegro...");
 DeInit();
 //FSOUND_Close();
 alfont_exit();
 allegro_exit();
 LogWrite ("OK.\n");
}

int Hardware::PreConfig (int argc, char *argv[])
{
     int n;
     if (argc > 1) LogWrite ("Analyzing command Line...\n");
     for (n = 1; n < argc; n++) {
         while (argv[n][0] == '-' || argv[n][0] == '/' ||
                argv[n][0] == 'r' || argv[n][0] == 'R')
               argv[n]++;
         switch (tolower(*argv[n])) {
            case 'f':
                 Config->ToggleWindowed(false);
                 LogWrite ("Opcion -f: Fullscreen\n");
                 break;
            case 'w':
                 Config->ToggleWindowed(true);
                 LogWrite ("Opcion -w: Windowed\n");
                 break;
            case 'm':
                 Config->ToggleSonido(false);
                 LogWrite ("Opcion -m: Mute\n");
                 break;
            case 's':
                 Config->ToggleSonido(true);
                 LogWrite ("Opcion -s: Sonido activado\n");
                 break;
            case 'n':
                 Config->ToggleBlending(false);
                 LogWrite ("Opcion -n: Blending desactivado\n");
                 break;
            case 'b':
                 Config->ToggleBlending(true);
                 LogWrite ("Opcion -b: Blending activado\n");
                 break;
            case 'z':
                 Config->IncGraphDetail (-10);
                 LogWrite ("Opcion -z: Desactivados efectos visuales");
                 break;
            case 'l':
                 Config->ToggleScanL (true);
                 LogWrite ("Opcion -l: Scanlines activadas\n");
                 break;
            case 'k':
                 Config->ToggleScanL (false);
                 LogWrite ("Opcion -k: Scanlines desactivadas\n");
                 break;
            case '1':
                 Config->ToggleRes(1);
                 LogWrite ("Opcion -r1: Resolucion 640 x 480\n");
                 break;
            case '2':
                 Config->ToggleRes(2);
                 LogWrite ("Opcion -r2: Resolucion 800 x 600\n");
                 break;
            case '3':
                 Config->ToggleRes(3);
                 LogWrite ("Opcion -r3: Resolucion 1024 x 768\n");
                 break;
            case 'h':
                 allegro_message ("Capitan Sevilla, El Remake\nComputerEmuZone Games Studio\n---------------------------\n"
                                  "\n-f Fullscreen\n-w Windowed\n-m Mute sound\n-s Sound on\n-b Enable Blending effects\n"
                                  "-n Disable Blending effects\n-l Enable ScanLines\n-k Disable ScanLines\n"
                                  "-z Disable all visual effects\n-rX (X is a number between 1 & 3) Choose resolution (1 = 640 x 480, 2 = 800 x 600, 3 = 1024 x 768)\n"
                                  "-h Show this help\n\nExample:\n\nCapitan -f -s -b -r2\n"
                                  "Run in 800 x 600 fullscreen, with sound, scanlines and Blending effects enabled");
                 LogWrite ("OpciÃ³n -h: Mostrando mensaje de ayuda\n");
                 return 1;
            }
         }
     return 0;
}

int Hardware::InitGraph ()
{
   if (Config->VerWindowed()) ModoGraf = GFX_AUTODETECT_WINDOWED;
     else
    ModoGraf = GFX_AUTODETECT_FULLSCREEN;

   LogWrite ("Iniciando modo grÃ¡fico: ");

#ifndef GP2X
   set_color_depth(Config->VerCDepth());
   if (set_gfx_mode(ModoGraf,Config->VerResAncho(),Config->VerResAlto(),0,0)) {
      set_color_depth(16);
      if (set_gfx_mode(ModoGraf,Config->VerResAncho(),Config->VerResAlto(),0,0)) {
         set_color_depth(32);
         if (set_gfx_mode(ModoGraf,Config->VerResAncho(),Config->VerResAlto(),0,0))
            if (set_gfx_mode (GFX_SAFE, Config->VerResAncho(), Config->VerResAlto(), 0,0)!=0) {
               allegro_message ("Error activando modo grÃ¡fico.\nGraphic init error.");
               LogWrite ("Error (%s). No se puede iniciar modo grÃ¡fico",allegro_error);
               return 1;
         }
      }
   }

   if (screen->w < 640 || screen->h < 480 || get_color_depth() < 16) {
      LogWrite ("ERROR EN LA INICIALIZACIÃN GRÃFICA.\n");
      Config->ToggleRes(1);
      Config->ToggleWindowed (0);
      allegro_message ("Error en la inicializaciÃ³n grÃ¡fica.\nGraphic init error.\n");
      return 1;
      }

#endif

#ifdef GP2X
   set_color_depth(16);
   if (set_gfx_mode(GFX_AUTODETECT,320,240,0,0))
	return z1;
#endif

   if ((is_windowed_mode() && !Config->VerWindowed()) ||
       (!is_windowed_mode() && Config->VerWindowed()))
            Config->ToggleWindowed();

   LogWrite ("%ix%i (W = %i) (CDepth = %i): %s\n", screen->w, screen->h, Config->VerWindowed(), get_color_depth(), gfx_driver->name);

   LocateBuffer();

   return 0;
}

void Hardware::TextSLang (const char *Texto, int x, int y, bool Center)
{
    if (Center) {
       alfont_textout_centre_aa_ex (screen, Fuente, Texto, x - 2, 94 + y, 0, -1);
       alfont_textout_centre_aa_ex (screen, Fuente, Texto, x + 2, 94 + y, 0, -1);
       alfont_textout_centre_aa_ex (screen, Fuente, Texto, x, 92 + y, 0, -1);
       alfont_textout_centre_aa_ex (screen, Fuente, Texto, x, 96 + y, 0, -1);

       alfont_textout_centre_aa_ex (screen, Fuente, Texto, x, 94 + y, makecol (252, 224, 168), -1);
       return;
       }

       alfont_textout_aa_ex (screen, Fuente, Texto, x - 2, 94 + y, 0, -1);
       alfont_textout_aa_ex (screen, Fuente, Texto, x + 2, 94 + y, 0, -1);
       alfont_textout_aa_ex (screen, Fuente, Texto, x, 92 + y, 0, -1);
       alfont_textout_aa_ex (screen, Fuente, Texto, x, 96 + y, 0, -1);

       alfont_textout_aa_ex (screen, Fuente, Texto, x, 94 + y, makecol (252, 224, 168), -1);

}

int Hardware::SelectLang()
{
    int ContLang = 0;
    int SB = 0;
    int Key = 0;
    char Mess[256];

    DATAFILE *Objeto;
    char Fichero[255];

    while (!SMS->Init(ContLang++));

    if (--ContLang <= 1)
       return ContLang - 1;

    LogWrite ("Cargando fondo de Selección de lenguaje...");
    Objeto = load_datafile_object ("./data/bitmaps1.dat", "DFBM_BMLANG");
    if (Objeto) {
       draw_sprite (screen, (BITMAP*)Objeto->dat, (screen->w - 640) / 2, (screen->h - 480) / 2);
       unload_datafile_object (Objeto);
       LogWrite ("Ok.\n");
       }
     else
      return 1;

    SetFont_af ("TTF_GEEKBYTE", 20);

    TextSLang (SMS->ReadMens (2), screen->w / 2, 0);
    TextSLang ("==================================================", screen->w / 2, 20);

    for (int n = 0; n < ContLang && n < 10; n++) {
        SMS->InitMens (n);
        sprintf (Mess, "%i: %s", n, SMS->ReadMens(0));
        TextSLang (Mess, screen->w / 2 - 120, 50 + n * 20, false);
    }

    TextSLang ("==================================================", screen->w / 2, 252);
    TextSLang ("Copyleft 2009 - ComputerEmuZone Games Studio", screen->w / 2, 272);

    while ((Key & 0xff) < '0' || (Key & 0xff) > ('0' + ContLang - 1))
        if (keypressed())
            Key = readkey();
          else
            if (close_button_pressed)
                return 255;

    Config->ToggleIdioma ((Key & 0xff) - '0');

    clear_bitmap (screen);

    return 0;
}

void Hardware::LocateBuffer (int Ancho, int Alto)
{
    AnchoPantalla = Ancho;
    AltoPantalla = Alto;
#ifdef GP2X
    AnchoPantalla /= 2;
    AltoPantalla /= 2;
#endif
    Xor = (Config->VerResAncho() - AnchoPantalla) / 2;
    Yor = (Config->VerResAlto() - AltoPantalla) / 2;
}

int Hardware::LoadDF1()
{
    LogWrite ("Cargando archivo de datos LOADING.DAT: ");
    DataP = load_datafile ("./data/loading.dat");
    if (!DataP) {
       allegro_message ("                   ERROR DE E/S\n\n No se encuentra el archivo LOADING.DAT");
       set_gfx_mode(GFX_TEXT,0,0,0,0);
       LogWrite ("Falta el archivo de datos LOADING.DAT\n");
       return 1;
    }
    LogWrite ("OK.\n");

    LogWrite ("Cargando archivo de datos SPRITES.DAT: ");
    SpritesBM = load_datafile("./data/sprites.dat");
    if (!SpritesBM) {
      allegro_message ("                   ERROR DE E/S\n\n No se encuentra el archivo SPRITES.DAT");
      set_gfx_mode(GFX_TEXT,0,0,0,0);
      LogWrite ("Falta el archivo de datos SPRITES.DAT\n");
      return 1;
    }
    LogWrite ("OK.\n");

    return 0;
}

int Hardware::LoadDF2 ()
{
    ObjCargados = 0;

    cls (0);
    SetFont_af ("TTF_CAPITAN", 30);
    VuelcaBuffer();

    LogWrite ("Cargando archivo de datos SCREENS.DAT: ");
    ScreensBM = load_datafile_callback ("./data/screens.dat", IndicadorCarga);
    if (!ScreensBM) {
       allegro_message ("                   ERROR DE E/S\n\n No se encuentra el archivo SCREENS.DAT");
       set_gfx_mode(GFX_TEXT,0,0,0,0);
       LogWrite ("Falta el archivo de datos SCREENS.DAT\n");
       return 1;
    }
    LogWrite ("OK.\n");

    LogWrite ("Cargando archivo de datos BITMAPS0.DAT: ");
    BitMapsBM = load_datafile_callback ("./data/bitmaps0.dat", IndicadorCarga);
    if (!BitMapsBM) {
       allegro_message ("                   ERROR DE E/S\n\n No se encuentra el archivo BITMAPS0.DAT");
       set_gfx_mode(GFX_TEXT,0,0,0,0);
       LogWrite ("Falta el archivo de datos BITMAPS0.DAT\n");
       return 1;
    }
    LogWrite ("OK.\n");

    LogWrite ("Cargando archivo de datos SFX.DAT: ");
    SFx = load_datafile_callback ("./data/sfx.dat", IndicadorCarga);
    if (!BitMapsBM) {
       allegro_message ("                   ERROR DE E/S\n\n No se encuentra el archivo SFX.DAT");
       set_gfx_mode(GFX_TEXT,0,0,0,0);
       LogWrite ("Falta el archivo de datos SFX.DAT\n");
       return 1;
    }
    LogWrite ("OK.\n");

    return 0;
}

int Hardware::LoadDF3 ()
{
    LogWrite ("Cargando archivo de datos SCREENS.DAT: ");
    ScreensBM = load_datafile ("./data/screens.dat");
    if (!ScreensBM) {
       allegro_message ("                   ERROR DE E/S\n\n No se encuentra el archivo SCREENS.DAT");
       set_gfx_mode(GFX_TEXT,0,0,0,0);
       LogWrite ("Falta el archivo de datos SCREENS.DAT\n");
       return 1;
    }
    LogWrite ("OK.\n");

    LogWrite ("Cargando archivo de datos BITMAPS0.DAT: ");
    BitMapsBM = load_datafile ("./data/bitmaps0.dat");
    if (!BitMapsBM) {
       allegro_message ("                   ERROR DE E/S\n\n No se encuentra el archivo BITMAPS0.DAT");
       set_gfx_mode(GFX_TEXT,0,0,0,0);
       LogWrite ("Falta el archivo de datos BITMAPS0.DAT\n");
       return 1;
    }
    LogWrite ("OK.\n");

   return 0;
}

int Hardware::Init () {

    SpeccyColor [0] = makecol (0,0,0);
    SpeccyColor [1] = makecol (0,0,255);
    SpeccyColor [2] = makecol (255,0,0);
    SpeccyColor [3] = makecol (255,0,255);
    SpeccyColor [4] = makecol (0,255,0);
    SpeccyColor [5] = makecol (0,255,255);
    SpeccyColor [6] = makecol (255,255,0);
    SpeccyColor [7] = makecol (255,255,255);
    MyFont = font;
    Fondo = create_bitmap (640, 480);
    Sombras = create_bitmap (ANCHOPANTALLA, TAMTILE * TILESALTO);
    SombrasRec = create_bitmap (ANCHOPANTALLA, TAMTILE * TILESALTO);
    Linea = create_bitmap (ANCHOPANTALLA, 2);
    clear_to_color (Sombras, makecol (255,0,255));
    Iniciado = true;
    BufMargX = 0;
    BufMargY = 0;
    Buffer = create_bitmap (2 * BufMargX + 640, 2 * BufMargY + 480);
    ScanL = create_bitmap (640, 480);
    Buffer2 = create_bitmap (640, 480);
    clear_to_color (ScanL, makecol (255,0,255));
    for (int n = 0; n < ScanL->h; n+=2)
      line (ScanL, 0, n, ScanL->w, n, makecol (0,0,0));

    return 0;
}

void Hardware::DeInit ()
{
     if (!Iniciado) return;
     destroy_bitmap (Fondo);
     destroy_bitmap (Sombras);
     destroy_bitmap (SombrasRec);
     destroy_bitmap (Linea);
     destroy_bitmap (Buffer);
     destroy_bitmap (Buffer2);
     destroy_bitmap (ScanL);
     unload_datafile (BitMapsBM);
     unload_datafile (ScreensBM);
//     unload_datafile (SFx);
     unload_datafile (DataP);
     Unload_Sprites();
     DelFont_af();
     DelFont();
}

void Hardware::Unload_Sprites()
{
     if (!SpritesBM)
        return;
     unload_datafile (SpritesBM);
     SpritesBM =  NULL;
}

void Hardware::ReInit ()
{
     LogWrite ("Reiniciando...\n");
     DeInit();
     Init();
     if (LoadDF1() || LoadDF3())
        LogWrite ("ERROR!!!!\n");
      else
        LogWrite ("Reinicio OK.\n");
}

void Hardware::TextOut (const char *s, int x, int y, int c1, int c2, int c3, int b1, int b2, int b3) {
     int fondo, color;
     if (c3 < 0) {
        color = c1;
        fondo = c2;
        if (fondo>=0) fondo = SpeccyColor[fondo];
        if (color>=0) color = SpeccyColor[color];
        }
      else {
        color = makecol (c1, c2, c3);
        if (b1 > 0 && b2 > 0 && b3 > 0)
           fondo = makecol (b1, b2, b3);
         else
           fondo = -1;
        }
     textout_ex (Buffer, MyFont, s, BufMargX + x, BufMargY + y, color, fondo);
}

void Hardware::TextOut_aa (const char *s, int x, int y, int RI, int GI, int BI, int RB, int GB, int BB, void *Bitmap) {
     int color = makecol (RI, GI, BI);
     int fondo = -1;
     if (RB >= 0 && GB >= 0 && BB >= 0)
        fondo = makecol (RB, GB, BB);
     if (Bitmap == NULL) {
        Bitmap = (void *) Buffer;
        x += BufMargX;
        y += BufMargY;
     }

     alfont_textout_aa_ex ((BITMAP*)Bitmap, Fuente, s, x, y, color, fondo);
}

void Hardware::TextOutCentre (const char *s, int y, int color, int fondo) {
     int x;
     x = int (AnchoPantalla / 2);
     if (fondo>=0) fondo = SpeccyColor[fondo];
     if (color>=0) color = SpeccyColor[color];
     textout_centre_ex (Buffer, MyFont, s, 200+x, 200+y, color, fondo);
}

void Hardware::TextOutCentre_aa (const char *s, int y, int RI, int GI, int BI, int RB, int GB, int BB, void *Bitmap) {
     int x;
     int color = makecol (RI, GI, BI);
     int fondo = -1;
     if (Bitmap == NULL) {
        Bitmap = (void *) Buffer;
        x = AnchoPantalla / 2;
        y += BufMargY;
     }
      else
        x = ((BITMAP*)Bitmap)->h / 2;
     if (RB >= 0 && GB >= 0 && BB >= 0)
        fondo = makecol (RB, GB, BB);
     alfont_textout_centre_aa_ex ((BITMAP*)Bitmap, Fuente, s, x, y, color, fondo);
}

void Hardware::TextOutCentre_ex (const char *s, int x, int y, int color, int fondo) {
     if (fondo>=0) fondo = SpeccyColor[fondo];
     if (color>=0) color = SpeccyColor[color];
     textout_centre_ex (Buffer, MyFont, s, BufMargX + x, BufMargY + y, color, fondo);
}

void Hardware::TextOutCentre_aa_ex (const char *s, int x, int y, int RI, int GI, int BI, int RB, int GB, int BB) {
     int color = makecol (RI, GI, BI);
     int fondo = -1;
     if (RB >= 0 && GB >= 0 && BB >= 0)
        fondo = makecol (RB, GB, BB);
     alfont_textout_centre_aa_ex (Buffer, Fuente, s, BufMargX + x, BufMargY + y, color, fondo);
   }

void Hardware::TextoPartido (const char *s, int x, int y, int R1, int G1, int B1, int R2, int G2, int B2)
{
     BITMAP *BufferT;
     int color1, color2;
     BufferT = create_bitmap (TextHeight_af() + TextLength_af ((char*)s), TextHeight_af());
     blit (Buffer, BufferT, BufMargX + x - (BufferT->w / 2), BufMargY + y, 0, 0, BufferT->w, BufferT->h);
     color1 = makecol (R1, G1, B1);
     color2 = makecol (R2, G2, B2);

     alfont_textout_centre_aa_ex (Buffer, Fuente, s, BufMargX + x, BufMargY + y, color1, -1);
     alfont_textout_centre_aa_ex (BufferT, Fuente, s, BufferT->w / 2, 0, color2, -1);

     blit (BufferT, Buffer,
           0, TextHeight_af() / 2,
           BufMargX + x - BufferT->w / 2, BufMargY + y + TextHeight_af() / 2,
           BufferT->w, TextHeight_af() / 2);

     destroy_bitmap (BufferT);
}

void Hardware::TextNum (int Num, int x, int y, int color, int fondo)
{
     if (fondo>=0) fondo = SpeccyColor[fondo];
     if (color>=0) color = SpeccyColor[color];
     textprintf_ex (Buffer, MyFont, BufMargX + x, BufMargY + y, color, fondo, "%i", Num);
}

int Hardware::SetFont_af (const char *s1, int points)
{
     DATAFILE *PreFuente;

     DelFont_af();
     LogWrite ("Cargando fuente TTF (%s)..", s1);
     PreFuente = load_datafile_object ("./data/fonts.dat", s1);
     LogWrite (".");

     for (int n = 0; n < 20; n++) {
         FuenteTam[n] = alfont_load_font_from_mem((char*)PreFuente->dat, PreFuente->size);
         alfont_set_font_size (FuenteTam[n], 10 + 5 * n);
         }
     LogWrite ("Ok");
     unload_datafile_object (PreFuente);
     LogWrite (".\n");
     return ResizeFont_af (points);
}

void Hardware::DelFont_af () {
     if (!Fuente)
        return;
     LogWrite ("Liberando memoria de fuente TTF Cargada...");
     for (int n = 0; n < 20; n++)
         alfont_destroy_font (FuenteTam[n]);
     Fuente = NULL;
     LogWrite ("OK.");
}

int Hardware::ResizeFont_af (int Points)
{
     int Indice = (Points - 10) / 5;
     if (Indice > 20)
        Indice = 20;
     if (Indice < 0)
        Indice = 0;
     Fuente = FuenteTam[Indice];
     if (Points % 5)
        alfont_set_font_size (Fuente, Points);
     return 0;
}

void Hardware::SetFont (const char *s1, const char *s2) {
     PALETTE pal;
     DelFont();
     char *names[] = {(char*) s1,(char*) s2};
     MyFont = load_dat_font("./data/fonts.dat", pal, names);
     set_palette (pal);
     font = MyFont;
}

void Hardware::DelFont ()
{
     if (MyFont != font)
        destroy_font (MyFont);
     font = OldFont;
     MyFont = font;
}

void Hardware::cls (int color) {
     clear_to_color (Buffer, SpeccyColor[color]);}

void Hardware::DrawFondoBuffer (int x, int y, int LongX, int LongY, int xo, int yo)
{
     masked_blit (Fondo,Buffer,xo,yo,BufMargX + x,BufMargY + y,LongX,LongY);
}

void Hardware::VuelcaBuffer ()
{
     int TerreX, TerreY;

     if (close_button_pressed)
        return;

     ScanLines(20);

     if (Mamporrazo || MObjetivo) {
        int Dif = 6;
        if (Mamporrazo == MObjetivo ||
           (Mamporrazo < MObjetivo && Mamporrazo + Dif > MObjetivo) ||
           (Mamporrazo > MObjetivo && Mamporrazo - Dif < MObjetivo)
           )
           MObjetivo = Mamporrazo / -2 + Mamporrazo / -3;
        else
           if (Mamporrazo < MObjetivo)
              Mamporrazo += Dif;
            else
              if (Mamporrazo > MObjetivo)
                 Mamporrazo -= Dif;
        if (abs(MObjetivo) < Dif)
           Mamporrazo = 0;
        clear_bitmap (Buffer2);
        blit (Buffer, Buffer2, BufMargX, BufMargY, 0, Mamporrazo, ANCHOPANTALLA, ALTOPANTALLA);
        blit (Buffer2, screen, 0, 0, Xor, Yor, 640, 480);
        return;
        }
    TerreX = Terremot * ((int) (rand() % 8) - 4);
    TerreY = Terremot * ((int) (rand() % 8) - 4);

    vsync();
    blit (Buffer, screen,
          BufMargX + TerreX, BufMargY + TerreY,
          Xor, Yor,
          AnchoPantalla, AltoPantalla);
}

void Hardware::ScanLines (int Trans) {
     if (!Config->VerScanL())
        return;
     Dibuja_Trans (0, 0, (void*) ScanL, Trans);
}

void Hardware::RectFill (int x1, int y1, int x2, int y2, int color) {
     rectfill (Buffer, BufMargX+x1,BufMargY+y1,BufMargX+x2,BufMargY+y2,SpeccyColor[color]);
}

void Hardware::RectFill_ex (int x1, int y1, int x2, int y2, int color) {
     rectfill (Buffer, BufMargX+x1,BufMargY+y1,BufMargX+x2,BufMargY+y2,color);}


void Hardware::Circle_cut (int x, int y, int rad) {
     BITMAP *Temp;
     Temp = create_bitmap (320, 320);
     clear_to_color (Temp, 0);
     circlefill (Temp, x / 2, y / 2, rad, makecol (255, 0, 255));

     Dibuja2x (0, 0, Temp);
     destroy_bitmap (Temp);
}

void Hardware::SetFondo (int NumFondo)
{
     BITMAP *Temp;
     Temp = (BITMAP*)SpritesBM[NumFondo].dat;
     blit (Temp, Fondo, 0,0,0,0, Temp->w, Temp->h);
}

void Hardware::SetFondo_ex (int NumFondo)
{
     BITMAP *Temp;
     Temp = (BITMAP*)SpritesBM[NumFondo].dat;
     stretch_sprite (Fondo, Temp, 0, 0, Temp->w, Temp->h);
}

void Hardware::SetFondo (void* Backgrd)
{
     BITMAP *Temp;
     Temp = (BITMAP*)Backgrd;
     blit (Temp, Fondo, 0,0,0,0, Temp->w, Temp->h);
}

void Hardware::Dibuja2x (int x, int y, void* Spr)
{
     BITMAP *Temp;
     Temp = (BITMAP*)Spr;
     stretch_sprite (Buffer, Temp, BufMargX + x, BufMargY + y, 2*(Temp->w), 2*(Temp->h));
}

void Hardware::CopyFondo ()
{
     blit (Buffer, Fondo, BufMargX, BufMargY, 0, 0, AnchoPantalla + BufMargX, AltoPantalla + BufMargX);
}

void Hardware::ClrFondo (int Color) {
     clear_to_color (Fondo, SpeccyColor [Color]);}

void Hardware::Borra (int x, int y, int LongX, int LongY, int xm, int ym) {
     blit (Fondo,Buffer, x, y, BufMargX + x + xm, BufMargY + y + ym, LongX, LongY);
}

void Hardware::Dibuja (int x, int y, void* Spr, void* BitMap) {
     if (BitMap == NULL) {
        BitMap = Buffer;
        x += BufMargX;
        y += BufMargY;
     }

     draw_sprite ((BITMAP*) BitMap,(BITMAP*) Spr, x, y);
}

void Hardware::DibujaCentr (void *Spr, int x, int y)
{
     BITMAP *Temp = (BITMAP*) Spr;

     if (x < 0)
        x = AnchoPantalla / 2 - Temp->w / 2;
      else
        if (x > 0)
           x = AnchoPantalla - Temp->w;

     if (y < 0)
        y = AltoPantalla / 2 - Temp->h / 2;
      else
        if (y > 0)
           y = AltoPantalla - Temp->h;

     draw_sprite (Buffer, Temp, BufMargX + x, BufMargY + y);
}

void Hardware::Dibuja (const char* File, const char* Obj, int x, int y, void *BitMap)
{
     LogWrite ("Cargando %s en %s...", Obj, File);
     DATAFILE *Objeto;
     char Fichero[255];
     sprintf (Fichero, "./data/%s.dat", File);
     Objeto = load_datafile_object (Fichero, Obj);
     if (Objeto) {
        Dibuja (x, y, (void*)Objeto->dat, BitMap);
        unload_datafile_object (Objeto);
        }
     LogWrite ("Ok.\n");
}

void Hardware::Dibuja2x (const char* File, const char* Obj, int x, int y)
{
     LogWrite ("Cargando %s en %s...", Obj, File);
     DATAFILE *Objeto;
     char Fichero[255];
     sprintf (Fichero, "./data/%s.dat", File);
     Objeto = load_datafile_object (Fichero, Obj);
     if (Objeto) {
        Dibuja2x (x, y, (void*)Objeto->dat);
        unload_datafile_object (Objeto);
        }
     LogWrite ("Ok.\n");
}

void Hardware::DibujaCentr (const char* File, const char* Obj, int x, int y)
{
     LogWrite ("Cargando %s en %s...", Obj, File);
     DATAFILE *Objeto;
     char Fichero[255];
     sprintf (Fichero, "./data/%s.dat", File);
     Objeto = load_datafile_object (Fichero, Obj);
     if (Objeto) {
        DibujaCentr ((void*)Objeto->dat,x,y);
        unload_datafile_object (Objeto);
        }
     LogWrite ("Ok.\n");
}

void Hardware::Dibuja_Outline (const char *File, const char *Obj, int x, int y, int w)
{
     LogWrite ("Cargando %s en %s...", Obj, File);
     DATAFILE *Objeto;
     char Fichero[255];
     sprintf (Fichero, "./data/%s.dat", File);
     Objeto = load_datafile_object (Fichero, Obj);
     if (Objeto) {
        DibujaNegro (x - w, y, (void*)Objeto->dat);
        DibujaNegro (x + w, y, (void*)Objeto->dat);
        DibujaNegro (x, y - w, (void*)Objeto->dat);
        DibujaNegro (x, y + w, (void*)Objeto->dat);
        Dibuja (x, y, (void*)Objeto->dat);
        unload_datafile_object (Objeto);
        }
     LogWrite ("Ok.\n");
}

void Hardware::DibujaNegro (int x, int y, void *Spr)
{
     set_luminance_blender (0,0,0,255);
     draw_lit_sprite (Buffer, (BITMAP*)Spr, BufMargX + x, BufMargY + y, makecol (0,0,0));
     solid_mode();
}

void Hardware::DibujaHalo (int x, int y, void *Spr, int Color, bool Suelo)
{
     BITMAP *Temp;
     int W, H;
//     int Maximo;
     W = ((BITMAP*)Spr)->w;
     H = ((BITMAP*)Spr)->h;
     Temp = create_bitmap (W + 4, H + 2 + 2*Suelo);
     clear_to_color (Temp, makecol (0,0,0));
     if (Color < 250)
        set_add_blender(0, Color, 0, 255);
      else
        set_add_blender(255, Color, 255, 255);
     draw_lit_sprite (Temp,(BITMAP*) Spr, 4, 2, 255);
     draw_lit_sprite (Temp,(BITMAP*) Spr, 0, 2, 255);
     draw_lit_sprite (Temp,(BITMAP*) Spr, 2, 0, 255);
     draw_lit_sprite (Temp,(BITMAP*) Spr, 2, 4, 255);
     solid_mode();
     Dibuja_TransAdd (x - 2, y - 2, (void*)Temp, Color, true);
/*     Maximo = 0;
     if (get_color_depth() > 16)
        Maximo = 47;
     Color = 55 - Maximo;
     set_add_blender (0, Color, 0, Color);
     draw_lit_sprite (Buffer,(BITMAP*) Spr, x, y, 255);
     solid_mode();*/
     destroy_bitmap (Temp);
}

void Hardware::Dibuja_Trans  (int x, int y, int Trans, const char *File, const char *Obj, bool Force)
{
     DATAFILE *Objeto;
     char Fichero[255];
     sprintf (Fichero, "./data/%s.dat", File);
     Objeto = load_datafile_object (Fichero, Obj);
     if (Objeto) {
        Dibuja_Trans (x, y, (void*)Objeto->dat, Trans, Force);
        unload_datafile_object (Objeto);
        }
}

void Hardware::Dibuja_Trans (int x, int y, void* Spr, int Trans, bool Force) {
  if (Trans == 255) {
    Dibuja (x, y, Spr);
    return;
  }
  if (!Config->VerBlending() && !Force)
     return;

#ifdef FBLEND
  if (Trans < 120 || Trans > 128)
      fblend_trans ((BITMAP*) Spr, Buffer, BufMargX + x, BufMargY + y, Trans);
    else {
      set_trans_blender(0, 0, 0, Trans);
      draw_trans_sprite (Buffer, (BITMAP*)Spr, BufMargX + x, BufMargY + y);
      solid_mode();
    }
  return;
#endif

#ifndef FBLEND
  set_trans_blender(0, 0, 0, Trans);
  draw_trans_sprite (Buffer, (BITMAP*)Spr, BufMargX + x, BufMargY + y);
  solid_mode();
  return;
#endif
}

void Hardware::Dibuja_TransAdd (int x, int y, int Trans, const char *File, const char *Obj, bool Force)
{
     DATAFILE *Objeto;
     char Fichero[255];
     sprintf (Fichero, "./data/%s.dat", File);
     Objeto = load_datafile_object (Fichero, Obj);
     if (Objeto) {
        Dibuja_TransAdd (x, y, (void*)Objeto->dat, Trans, Force);
        unload_datafile_object (Objeto);
        }
}

void Hardware::Dibuja_TransAdd (int x, int y, void* Spr, int Trans, bool Force) {
  if (Trans == 255) {
    Dibuja (x, y, Spr);
    return;
    }

  if (!Config->VerBlending() && !Force)
     return;


  if (Trans <= 0)
     return;

#ifdef FBLEND // Blending con fblend
  fblend_add ((BITMAP *)Spr, Buffer, BufMargX + x, BufMargY + y, Trans);
  return;
#endif

#ifndef FBLEND // Blending con Allegro
  set_add_blender (0,0,0,Trans);
  draw_trans_sprite (Buffer, (BITMAP*)Spr, BufMargX + x, BufMargY + y);
  solid_mode();
  return;
#endif
}

void* Hardware::VerFondo()
{
      return (void*)Fondo;
}

void Hardware::Borra_Sombra (int x, int y, int LongX, int LongY)
{
     // Restaura el búffer de sombras borrando la sombra de un sprite, del que damos coordenadas y dimensiones
     rectfill (Sombras, x, y, x + LongX, y + LongY, makecol(255,0,255));
}

void Hardware::Dibuja_Sombra (int x, int y, void *Spr)
{
     set_luminance_blender (0,0,0,255);
     draw_lit_sprite (Sombras, (BITMAP*)Spr, x, y, makecol (0,0,0));
     solid_mode();
}

void Hardware::SetPuntoLuz (int x, int y)
{
     PuntoLuzX = x;
     PuntoLuzY = y;
}

int Hardware::ValidaVarSombra (int &Variable, int Profundo)
{
    if (Variable < 0) Variable = 0;
    if (Variable > Profundo) Variable = Profundo;
    return Variable;
}

void Hardware::BorraSombra (int x1, int y1, int x2, int y2)
{
     // Borra las sombras que pueda haber en un Ã¡rea del dibujo de sombras ya formado
     rectfill (SombrasRec, x1, y1, x2, y2, makecol(255,0,255));
}

void Hardware::RecortaSombra (int x1, int y1, int x2, int y2, int Profundo)
{
     int Ar, Ab, Iz, De;

     Ar = (Profundo / 2) - ((y1 + y2) / 2 - PuntoLuzY) / 10;
     Ab = (Profundo / 2) + ((y1 + y2) / 2 - PuntoLuzY) / 10;
     Iz = Profundo - ((x1 + x2) / 2 - PuntoLuzX) / 10;
     De = Profundo + ((x1 + x2) / 2 - PuntoLuzX) / 10;

     ValidaVarSombra (Ar, Profundo / 2);
     ValidaVarSombra (Ab, Profundo / 2);
     ValidaVarSombra (Iz, Profundo);
     ValidaVarSombra (De, Profundo);

     stretch_blit (Sombras, SombrasRec, x1 + Iz, y1 + Ar, x2 - De - x1 - Iz, y2 - Ab - y1 - Ar, x1, y1, x2 - x1, y2 - y1);
}

void Hardware::Vuelca_Sombra (int NumHab, int x, int y, int Transp)
{
     clear_to_color (SombrasRec, makecol (255,0,255));

     if (x > 0 && y > 0)
       SetPuntoLuz (x,y);

     RecortesSombras(x, y, NumHab);

     Dibuja_Trans (0, 0, SombrasRec, Transp);
}

void Hardware::RecortesSombras(int x, int y, int NumHab)
{
     if (y == 0)
        y = 0;    // <--------------------------------------------------

     switch (NumHab) {
            case 0:
                 if (x < 0)
                    SetPuntoLuz (416, 158);
                 RecortaSombra (92, 0, 354, 162, 20);
                 RecortaSombra (92, 174, 354, 256, 20);
                 RecortaSombra (484, 0, 512, 256,10);
                 break;
            case 1:
                 if (x < 0)
                   SetPuntoLuz (28, 78);
                 RecortaSombra (46, 124, 82, 256, 25);  // Muro del fondo donde está la señal
                 RecortaSombra (94, 172, 288, 256, 20); // Casa baja
                 if (x < 0)
                   SetPuntoLuz (272, 30);
                 RecortaSombra (94, 128, 288, 165, 20); // Tejado de la casa baja
                 RecortaSombra (289, 0, 512, 256, 10);  // Bar "er de Triana"
                 BorraSombra (260, 172, 285, 201);      // Letrero de cocacola
                 break;
            case 2:
                 if (x < 0)
                   SetPuntoLuz (140, 45);
                 RecortaSombra (0, 0, 161, 256, 10);     // Primera casa
                 RecortaSombra (162, 82, 337, 256, 20);  // Segunda casa
                 RecortaSombra (421, 187, 447, 213, 15); // Señal de direc Prohibida
                 RecortaSombra (448, 0, 512, 256, 15);   // Tercera casa
                 break;
            case 3:
                 if (x < 0)
                   SetPuntoLuz (172, 108);
                 RecortaSombra (0, 0, 288, 256, 10);     // ConfiterÃ­a
                 if (x < 0)
                   SetPuntoLuz (416, 156);
                 RecortaSombra (288, 122, 384, 158, 20); // Tejado de la casa
                 RecortaSombra (288, 168, 371, 256, 20); // Casa
                 break;
            case 4:
                 if (x < 0)
                   SetPuntoLuz (48, 156);
                 RecortaSombra (0, 0, 27, 256, 20);      // Casa Lateral
                 RecortaSombra (78, 124, 144, 158, 20);  // Casa primer macetero Tejado
                 RecortaSombra (78, 168, 144, 256, 20);  // Casa primer macetero
                 RecortaSombra (144, 0, 336, 162, 10);   // Casa central arriba
                 RecortaSombra (144, 170, 336, 256, 10); // Casa central abajo
                 RecortaSombra (172,194, 302, 210, 5);   // Parte abajo toldo
                 RecortaSombra (178, 176, 296, 194, 10); // Parte arriba toldo
                 RecortaSombra (336, 124, 400, 158, 20); // Casa segundo macetero Tejado
                 RecortaSombra (336, 168, 400, 256, 20); // Casa segundo macetero
                 break;
            case 5:
                 if (x < 0)
                   SetPuntoLuz (30, 30);         // La luna
                 RecortaSombra (0, 110, 288, 256, 20);   // Abajo izqda.
                 RecortaSombra (288, 0, 512, 256, 20);   // Muro Ladrillos
                 RecortaSombra (62, 0, 110, 110, 10);    // Valla arriba izqda.
                 BorraSombra (192, 224, 290, 256);       // Vigas...
                 BorraSombra (256, 192, 512, 226);
                 BorraSombra (418, 128, 512, 160);
                 BorraSombra (226, 80, 378, 112);
                 BorraSombra (0, 128, 222, 160);
                 BorraSombra (0, 80, 126, 112);
                 break;
            case 6:
                 if (x < 0)
                   SetPuntoLuz (30, 30);         // La luna
                 RecortaSombra (258, 152, 480, 238, 10);
                 BorraSombra (160, 240, 512, 274);
                 BorraSombra (128, 144, 350, 176);
                 BorraSombra (0, 112, 128, 144);
                 break;
            case 7:
                 if (x < 0)
                   SetPuntoLuz (30, 30);         // La luna
                 RecortaSombra (30, 40, 144, 142, 20);
                 RecortaSombra (100, 78, 152, 140, 10);
                 RecortaSombra (156, 66, 214, 137, 5);
                 RecortaSombra (324, 60, 412, 132, 20);
                 break;
            case 8:
                 if (x < 0)
                   SetPuntoLuz (30, 30);         // La luna
                 RecortaSombra (0, 162, 32, 190, 10);
                 RecortaSombra (0, 224, 32, 256, 10);
                 RecortaSombra (192, 128, 224, 256, 10);
                 RecortaSombra (320, 130, 352, 192, 10);
                 RecortaSombra (320, 226, 352, 304, 10);
                 break;
            case 9:
                 if (x < 0)
                   SetPuntoLuz (30, 30);         // La luna
                 RecortaSombra (0, 64, 32, 242, 10);
                 RecortaSombra (192, 18, 224, 96, 10);
                 RecortaSombra (320, 0, 342, 176, 10);
                 RecortaSombra (224, 210, 256, 272, 10);
                 break;
            case 10:
                 if (x < 0)
                    SetPuntoLuz (30, 30);         // La luna
                 RecortaSombra (0, 64, 128, 256, 10);
                 BorraSombra (0, 128, 158, 160);
                 RecortaSombra (128, 175, 224, 256, 20);
                 RecortaSombra (224, 162, 448, 256, 10);
                 RecortaSombra (448, 162, 512, 256, 20);
                 RecortaSombra (224, 66, 320, 128, 10);
                 RecortaSombra (320, 82, 482, 128, 20);
                 break;
            case 11:
                 if (x < 0)
                   SetPuntoLuz (48,176);                  // Primera farola
                 RecortaSombra (0, 90, 358, 256, 20);
                 if (x < 0)
                   SetPuntoLuz (494, 176);                // Segunda farola
                 RecortaSombra (358, 90, 512, 256, 10);
                 BorraSombra (298, 68, 410, 256);       // La estatua no tiene sombra pq estÃ¯Â¿Â½a la misma altura
                 break;
            case 12:
                 if (x < 0)
                   SetPuntoLuz (80, 176);                  // Primera farola
                 RecortaSombra (0, 138, 176, 256, 10);
                 if (x < 0)
                   SetPuntoLuz (270, 176);                  // Segunda farola
                 RecortaSombra (176, 146, 512, 256, 10);
                 RecortaSombra (386, 166, 450, 210, 20);  // Camion
                 break;
            case 13:
                 if (x < 0)
                   SetPuntoLuz (30, 30);         // La luna
                 RecortaSombra (0, 106, 270, 256, 10);
                 RecortaSombra (270, 208, 512, 256, 20);
                 BorraSombra (56, 214, 122, 256); // El pedrusco;
                 break;
            case 14:
                 if (x < 0)
                   SetPuntoLuz (30, 30);         // La luna
                 RecortaSombra (0, 90, 356, 256, 20);
                 RecortaSombra (214, 170, 305, 244, 20);
                 break;
            case 15:
                 if (x < 0)
                   SetPuntoLuz (30, 30);         // La luna
                 RecortaSombra (184, 0, 414, 272, 30);
                 break;
            case 16:
                 break;
            case 17:
                 break;
            case 18:
                 if (x < 0)
                   SetPuntoLuz (ANCHOPANTALLA / 2, 0);
                 RecortaSombra (378,0, 512, 256, 20);
                 break;
            case 19:
                 if (x < 0)
                   SetPuntoLuz (ANCHOPANTALLA / 2, 0);
                 RecortaSombra (0, 0, 512, 256, 10);
                 RecortaSombra (34, 106, 76, 256, 20);
                 RecortaSombra (340, 106, 382, 256, 20);
                 break;
            case 20:
                 if (x < 0)
                   SetPuntoLuz (ANCHOPANTALLA / 2, 0);
                 RecortaSombra (0, 0, 512, 256, 10);
                 RecortaSombra (34, 106, 76, 256, 20);
                 RecortaSombra (266, 106, 308, 256, 20);
                 RecortaSombra (340, 106, 382, 256, 20);
                 break;
            case 21:
                 if (x < 0)
                   SetPuntoLuz (470, 86);
                 RecortaSombra (14, 0, 508, 256, 30);            // Fondo
                 RecortaSombra (122, 0, 398, 60, 10);            // Parte Arriba
                 RecortaSombra (114, 60, 404, 190, 10);          // Parte Abajo
                 RecortaSombra (116, 190, 180, 246, 20);         // Salida cohete 1
                 RecortaSombra (226, 190, 292, 246, 20);         // Salida cohete 2
                 RecortaSombra (336, 190, 180, 346, 20);         // Salida cohete 3
                 RecortaSombra (48, 148, 112, 208, 20);          // Ala Izqda
                 RecortaSombra (404, 148, 504, 208, 20);         // Ala Dcha
                 RecortaSombra (188, 164, 218, 188, 5);          // Cohetillo 1
                 RecortaSombra (302, 164, 332, 188, 5);          // Cohetillo 2
                 break;
            case 22:
                 if (x < 0)
                   SetPuntoLuz (462, 166);
                 RecortaSombra (14, 0, 496, 272, 30);             // Fondo
                 RecortaSombra (172, 0, 346, 272, 10);            // Cohete
                 RecortaSombra (122, 34, 172, 272, 20);
                 RecortaSombra (346, 34, 400, 272, 20);
                 break;
            case 23:
                 if (x < 0)
                   SetPuntoLuz (454, 78);
                 RecortaSombra (14, 0, 496, 272, 20);             // Fondo
                 RecortaSombra (232, 98, 290, 272, 10);
                 RecortaSombra (240, 48, 284, 90, 30);
                 RecortaSombra (140, 48, 44, 90, 30);
                 BorraSombra (326, 256, 512, 272);                // Plataforma
                 break;
            case 25:
                 if (x < 0)
                   SetPuntoLuz ();
                 RecortaSombra (0, 200, 84, 260, 20);
                 RecortaSombra (146, 210, 260, 260, 20);
                 RecortaSombra (400, 162, 512, 260, 20);
                 BorraSombra (146, 210, 194, 227);
                 BorraSombra (146, 226, 450, 209);
                 BorraSombra (400, 162, 450, 209);
                 BorraSombra (400, 162, 431, 225);
                 break;
            case 26:
                 if (x < 0)
                   SetPuntoLuz (0,66);
                 RecortaSombra (0, 162, 78, 260, 10);
                 RecortaSombra (242, 162, 512, 260, 10);
                 RecortaSombra (78, 242, 242, 260, 5);
                 RecortaSombra (78, 226, 242, 242, 10);
                 RecortaSombra (78, 210, 242, 226, 15);
                 RecortaSombra (78, 194, 242, 210, 20);
                 RecortaSombra (78, 178, 242, 194, 25);
                 RecortaSombra (78, 162, 242, 178, 30);
                 RecortaSombra (418, 128, 445, 260, 5);
                 break;
            case 27:
                 if (x < 0)
                   SetPuntoLuz ();
                 RecortaSombra (0, 162, 215, 260, 20);
                 RecortaSombra (216, 128, 512, 256, 10);
                 break;
            case 28:
                 if (x < 0)
                   SetPuntoLuz ();
                 RecortaSombra (0, 128, 512, 256, 20);
                 BorraSombra (120, 128, 162, 256);
                 BorraSombra (328, 128, 370, 256);
                 BorraSombra (162, 128, 328, 145);
                 break;
            case 33:
                 if (x < 0)
                   SetPuntoLuz ();
                 RecortaSombra (0, 0, ANCHOPANTALLA, (TILESALTO) * TAMTILE, 20);
                 break;
            case 34:
                 if (x < 0)
                   SetPuntoLuz ();
                 RecortaSombra (0, 18, 512, 207, 20);
                 RecortaSombra (74, 18, 148, 207, 10);
                 RecortaSombra (90, 42, 105, 190, 10);
                 RecortaSombra (378, 18, 450, 207, 10);
                 RecortaSombra (390, 42, 405, 190, 10);
                 break;
            case 35:
                 if (x < 0)
                   SetPuntoLuz ();
                 RecortaSombra (0, 18, 512, 207, 20);
                 RecortaSombra (40, 18, 195, 207, 10);
                 RecortaSombra (170, 18, 245, 207, 10);
                 RecortaSombra (170, 128, 245, 207, 5);
                 RecortaSombra (184, 42, 199, 127, 10);
                 RecortaSombra (184, 148, 199, 177, 5);
                 RecortaSombra (396, 128, 466, 207, 5);
                 RecortaSombra (402, 128, 422, 177, 5);
                 break;
            case 36:
                 if (x < 0)
                   SetPuntoLuz ();
                 RecortaSombra (0, 0, ANCHOPANTALLA, (TILESALTO) * TAMTILE, 20);
                 break;
            case 37:
                 if (x < 0)
                   SetPuntoLuz ();
                 RecortaSombra (18, 0, 43, 255, 10);
                 RecortaSombra (56, 158, 303, 255, 10);
                 RecortaSombra (134, 170, 203, 255, 20);
                 RecortaSombra (226, 170, 295, 255, 20);
                 RecortaSombra (180, 0, 233, 157, 20);
                 break;
            case 38:
                 if (x < 0)
                   SetPuntoLuz ();
                 RecortaSombra (118, 32, 205, 139, 20);
                 RecortaSombra (206, 32, 261, 137, 10);
                 RecortaSombra (261, 45, 287, 128, 10);
                 RecortaSombra (291, 60, 324, 110, 10);
                 RecortaSombra (326, 68, 433, 101, 20);
                 RecortaSombra (138, 198, 199, 255, 10);
                 break;
/*            default:
                 SetPuntoLuz();
                 RecortaSombra (0, 0, ANCHOPANTALLA, (TILESALTO) * TAMTILE, 20);*/
            }
}

int Hardware::Tecla (bool Menu) {
     int TeclaPulsada = 0;

     if (Config->VerJoy()) {
         poll_joystick();
	     if (joy[0].stick[0].axis[0].pos/40 < 0)
	        TeclaPulsada += IZQUIERDA;
         if (joy[0].stick[0].axis[0].pos/40 > 0)
	        TeclaPulsada += DERECHA;
         if (joy[0].stick[0].axis[1].pos/40 < 0 || joy[0].button[2].b || joy[0].button[3].b)
            TeclaPulsada += ARRIBA;
         if (joy[0].stick[0].axis[1].pos/40 > 0)
            TeclaPulsada += ABAJO;

         if (joy[0].button[0].b && SueltaTecla[0]) {
            TeclaPulsada += FUEGO;
            SueltaTecla[0] = false;
            }
         if (!joy[0].button[0].b && (Config->VerJoy() == 1 || !key[Fuego]))
            SueltaTecla[0] = true;

         if (joy[0].button[1].b && SueltaTecla[1]) {
            TeclaPulsada += SELEC;
            SueltaTecla[1] = false;
            }
         if (!joy[0].button[1].b && (Config->VerJoy() == 1 || !key[Selec]))
            SueltaTecla[1] = true;

         if ((joy[0].button[6].b || joy[0].button[7].b)
            && SueltaTecla[3]) {
            TeclaPulsada += ABORT;
            SueltaTecla[3] = false;
            }
         if (!joy[0].button[1].b && !joy[0].button[7].b && (Config->VerJoy() == 1 || !key[Abort]))
             SueltaTecla[3] = true;
         }

     if (key[KEY_S] && SueltaTecla[2]) { // <------------------------------ BETA TESTING
        ScreenShot();
        SueltaTecla[2] = false;
        }
     if (!key[KEY_S]) SueltaTecla[2] = true;

     if (key[Abort] && SueltaTecla[3]) {
        TeclaPulsada += ABORT;
        SueltaTecla[3] = false;
        }
     if (!key[Abort]) SueltaTecla[3] = true;

     if (Config->VerJoy() == 1) // Si es 1 es que solo funciona el joystick y el teclado no
        return TeclaPulsada;

     if (key[Arriba]) TeclaPulsada += ARRIBA;
     if (key[Abajo] && !key[Arriba]) TeclaPulsada += ABAJO;
     if (key[Izquierda]&& !key[Abajo]) TeclaPulsada += IZQUIERDA;
     if (key[Derecha] && !key[Izquierda] && !key[Abajo]) TeclaPulsada += DERECHA;

     if (key[Fuego] && SueltaTecla[0]) {
        TeclaPulsada += FUEGO;
        SueltaTecla[0] = false;
        }
     if (!key[Fuego] && (!Config->VerJoy() || !joy[0].button[0].b))
        SueltaTecla[0] = true;

     if (key[Selec] && SueltaTecla[1]) {
        TeclaPulsada += SELEC;
        SueltaTecla[1] = false;
        }
     if (!key[Selec] && (!Config->VerJoy() || !joy[0].button[1].b) && !key[KEY_ENTER])
              SueltaTecla[1] = true;

     if (!TeclaPulsada && Menu) {
        if (key[KEY_UP])
           TeclaPulsada += ARRIBA;
        if (key[KEY_DOWN] && !key[KEY_UP])
           TeclaPulsada += ABAJO;
        if (key[KEY_LEFT])
           TeclaPulsada += IZQUIERDA;
        if (key[KEY_RIGHT]  && !key[KEY_LEFT])
           TeclaPulsada += DERECHA;
        if (key[KEY_ENTER] && SueltaTecla[1]) {
           TeclaPulsada += SELEC;
           SueltaTecla[1] = false;
           }
        }

     return TeclaPulsada;
}

int Hardware::VerTecla () {
    if (!keypressed()) {
       if (key[KEY_LSHIFT])
          return KEY_LSHIFT << 8;
       if (key[KEY_RSHIFT])
          return KEY_RSHIFT << 8;
       if (key[KEY_LCONTROL])
          return KEY_LCONTROL << 8;
       if (key[KEY_RCONTROL])
          return KEY_RCONTROL << 8;
       if (key[KEY_ALT])
          return KEY_ALT << 8;
       if (key[KEY_ALTGR])
          return KEY_ALTGR << 8;
       if (key[KEY_SCRLOCK])
          return KEY_SCRLOCK << 8;
       if (key[KEY_NUMLOCK])
          return KEY_NUMLOCK << 8;
       if (key[KEY_CAPSLOCK])
          return KEY_CAPSLOCK << 8;
       return -1;
       }
    return readkey();
}

const int Hardware::VerTecla (int Sentido) {
    switch (Sentido) {
           case ARRIBA:
                return Arriba;
           case ABAJO:
                return Abajo;
           case IZQUIERDA:
                return Izquierda;
           case DERECHA:
                return Derecha;
           case FUEGO:
                return Fuego;
           case SELEC:
                return Selec;
           case ABORT:
                return Abort;}
    return -1;
}

bool Hardware::KeyPressed()
{
     if (keypressed())
        return true;

     if (Config->VerJoy()) {
        poll_joystick();
        for (int n = 0; n < 12; n++)
            if (joy[0].button[n].b)
               return true;
        }

     return false;
}

void Hardware::GrabarTecla (int Sentido, int Tecla) {
    switch (Sentido) {
           case ARRIBA:
                Arriba = Tecla >> 8;
                break;
           case ABAJO:
                Abajo = Tecla >> 8;
                break;
           case IZQUIERDA:
                Izquierda = Tecla >> 8;
                break;
           case DERECHA:
                Derecha = Tecla >> 8;
                break;
           case FUEGO:
                Fuego = Tecla >> 8;
                break;
           case SELEC:
                Selec = Tecla >> 8;
                break;
           case ABORT:
                Abort = Tecla >> 8;
                break;
           }
    Config->GrabaTecla (Sentido, Tecla >> 8);
}

void Hardware::EsperaTecla () {
     ClearKeyBuf();
     while (!keypressed()) {
           if (Config->VerJoy()) {
              poll_joystick();
              for (int n = 0; n < 12; n++)
                  if (joy[0].button[n].b)
                     return;
             }
           if (close_button_pressed)
              return;
           }
     return;
}

void Hardware::Rest (int Ms) {
     rest (Ms);}

int Hardware::TextHeight () {
    return text_height (font);}

int Hardware::TextHeight_af () {
    return alfont_get_font_height (Fuente);}

int Hardware::TextLength (const char *String) {
    return text_length (font, String);}

int Hardware::TextLength_af (const char *String) {
    return alfont_text_length (Fuente, String);}

void Hardware::ClearKeyBuf () {
     if (Config->VerJoy())
        poll_joystick();
     clear_keybuf();}

bool Hardware::Esc ()
{
     bool Ret = key[KEY_ESC];

     if (Config->VerJoy()) {
        poll_joystick();
        for (int n = 1; n < 12; n++)
            if (joy[0].button[n].b)
               return true;
        }

     if (Ret)
       while (key[KEY_ESC])
             clear_keybuf();

     return (Ret || close_button_pressed);
}

void Hardware::Fundido (int Sentido, int Vel, bool Sound) {
    switch (Sentido) {
           case -1:
                for (int n = 0; n < 255 && !Esc(); n += Vel) {
                    DrawFondoBuffer(0, 0, Config->VerResAncho(), Config->VerResAlto());
                    Vela (n, 0, 0, Config->VerResAncho(), Config->VerResAlto());
                    VuelcaBuffer();
                    if (Sound && Config->VerVolMus() - n >= 0)
                        set_volume (Config->VerVolMus() - n, 0);
                }
                Vela (255, 0, 0, Config->VerResAncho(), Config->VerResAlto());
                VuelcaBuffer();
                set_volume (Config->VerVolMus(), 0);
                break;
           case 1:
                for (int n = 255; n > 0 && !Esc(); n -= Vel) {
                    DrawFondoBuffer(0, 0, Config->VerResAncho(), Config->VerResAlto());
                    Vela (n, 0, 0, Config->VerResAncho(), Config->VerResAlto());
                    VuelcaBuffer();
                    if (Sound)
                        if (Config->VerVolMus() - n >= 0)
                            set_volume (Config->VerVolMus() - n, 0);
                          else
                            set_volume (0,0);
                }
                set_volume (Config->VerVolMus(), 0);
                DrawFondoBuffer(0, 0, Config->VerResAncho(), Config->VerResAlto());
                VuelcaBuffer();
                break;
    }
}

void Hardware::FundidoBM (int Vel)
{
     BITMAP *BitMap;
     BitMap = create_bitmap (AnchoPantalla, AltoPantalla);
     blit (Buffer, BitMap, BufMargX, BufMargY, 0, 0, AnchoPantalla, AltoPantalla);

     for (int n = 0; n < 255/* && !Esc()*/; n += Vel) {
         DrawFondoBuffer(0, 0, Config->VerResAncho(), Config->VerResAlto());
         Dibuja_Trans (0,0,(void*)BitMap,n);
         VuelcaBuffer();
         }
     Dibuja (0, 0, BitMap);
     CopyFondo();
     VuelcaBuffer();

     destroy_bitmap (BitMap);
}

void Hardware::TransMode(int n) {
    drawing_mode (DRAW_MODE_TRANS,NULL,0,0);
    set_trans_blender(0,0,0,n);
}

void Hardware::SolidMode()
{
       solid_mode();
}

void Hardware::Vela (int Trans, int X, int Y, int W, int H)
{
     if (!Config->VerBlending()) {
        if (Trans > 128)
           RectFill (X, Y, X + W, Y + H, 0);
        return;
        }
#ifndef FBLEND
    TransMode(Trans);
    rectfill (Buffer, BufMargX + X, BufMargY + Y, BufMargX + X + W, BufMargY + Y + H, 0);
    SolidMode();
    return;
#endif

    if (Trans < 0)
       Trans = 0;
    if (Trans > 254)
       Trans = 254;

    if (get_color_depth() < 32 && Trans > 245)
       Trans = 245;
#ifdef FBLEND
    fblend_rect_trans (Buffer, BufMargX + X, BufMargY + Y, W, H, 0, Trans);
#endif
}

void Hardware::Ilumina (int Trans, int X, int Y, int W, int H)
{
     if (!Config->VerBlending()) {
        if (Trans > 128)
           RectFill (X, Y, X + W, Y + H, makecol (255, 255, 255));
        return;
        }
#ifndef FBLEND
    drawing_mode (DRAW_MODE_TRANS,NULL,0,0);
    set_add_blender(255, 255, 255, Trans);
    rectfill (Buffer, BufMargX + X, BufMargY + Y, BufMargX + X + W, BufMargY + Y + H, makecol (255,255,255));
    SolidMode();
    return;
#endif

    if (Trans < 0)
       Trans = 0;
    if (Trans > 254)
       Trans = 254;

    if (get_color_depth() < 32 && Trans > 245)
       Trans = 245;
#ifdef FBLEND
    fblend_rect_add (Buffer, BufMargX + X, BufMargY + Y, W, H, makecol (255,255,255), Trans);
#endif
}

void Hardware::Desatura (int Transp)
{
    BITMAP *Temp;
    Temp = create_bitmap (Buffer->w, Buffer->h);
    clear_bitmap (Temp);
    drawing_mode (DRAW_MODE_TRANS,NULL,0,0);
    set_saturation_blender(0, 0, 0, Transp);
    draw_trans_sprite (Temp, Buffer, 0, 0);
    solid_mode();
    draw_sprite (Buffer, Temp, 0, 0);
    destroy_bitmap (Temp);
}


void* Hardware::CargaSprite (int Descrip)
{
      return (void*) SpritesBM [Descrip].dat;
}

void* Hardware::CargaBitMap (int Descrip)
{
      return (void*) BitMapsBM [Descrip].dat;
}

void* Hardware::CargaScreen (int Screen)
{
      return (void*) ScreensBM [Screen].dat;
}

char* Hardware::VerVersion ()
{
      return Version;
}

int Hardware::ScreenShot ()
{
    char title[14];
    int Ret;
    BITMAP *SShot;

    SShot = create_bitmap (AnchoPantalla, AltoPantalla);

    if (ActScreenShot == 1000) ActScreenShot = 999;

    sprintf (title, "CAPSEV");
    if (ActScreenShot < 100)
       sprintf (title, "%s0",title);
    if (ActScreenShot < 10)
       sprintf (title, "%s0",title);
    sprintf (title, "%s%i.png", title, ActScreenShot++);

    blit (screen, SShot, Xor, Yor, 0, 0, AnchoPantalla, AltoPantalla);
    LogWrite ("Grabando ScreenShot: %s", title);

    Ret = save_png (title, SShot, NULL);

    destroy_bitmap (SShot);

    return Ret;
}

int Hardware::MapLoadInit()
{
    MapaDat = load_datafile("./data/map.dat");
    if (!MapaDat) {
       allegro_message ("                   ERROR DE E/S\n\n No se encuentra el archivo MAP.DAT");
       set_gfx_mode(GFX_TEXT,0,0,0,0);
       LogWrite ("Falta el archivo de datos MAP.DAT\n");
       return 1;
    }
    PuntMapa = 6;
    Mapa = (unsigned char*)MapaDat[0].dat;
    return 0;
}

unsigned char Hardware::MapLoadChar()
{
    unsigned char Ret;
    Ret = (unsigned char) Mapa[PuntMapa++];
    return Ret;
}

int Hardware::MapLoadClose ()
{
    unload_datafile (MapaDat);
    return 0;
}

void Hardware::HeatDist ()
{
     if (++ContHD > 2) {
	if (++HDDesplC > 64)
	   HDDesplC = 0;
        ContHD = 0;
        }
     for (int i = 0; i < 64; i ++) {
         blit (Buffer, Linea, BufMargX, BufMargY + 272 - i * 2, 0, 0, AnchoPantalla, 2); // Tomamos una linea de buffer
         Dibuja_Trans (BufMargY + HDDespl[HDDesplC][i], BufMargY + 272 - 2 * i, (void*) Linea, 128 - i * 2);
         }
}

char* Hardware::NombreTecla (int Tecla)
{
    return ((char*) scancode_to_name(Tecla));
}

char* Hardware::Input (int x, int y, int Length, bool Centr, int R, int G, int B, int RB, int GB, int BB)
{
  bool Salir = false;
  int Tecla, Long;
  Temp[0] = 0;
  Long = 0;
  Tecla = 0;

  while (!Salir) {                              // y este se encarga del "Input"
        Tecla = VerTecla() & 0xff;
        if ((Tecla == 13) && Long)
           Salir = true;
        if ((Tecla == 8) && Long)
           Temp [--Long] = '\000';
        if ((Tecla >= '0' && Tecla <= 'z') && Long < Length) {
           Temp [Long++] = Tecla;
           Temp [Long] = '\000';}
        if (Tecla > 0) {
           DrawFondoBuffer(0, 0, Config->VerResAncho(), Config->VerResAlto());
           if (Centr)
              TextOutCentre_aa_ex (Temp, x, y, R, G, B, RB, GB, BB);
            else
              TextOut_aa (Temp, x, y, R, G, B, RB, GB, BB);
           VuelcaBuffer();
           }
  }

  return Temp;
}

void* Hardware::DatSFx ()
{
      return (void*)SFx;
}

void Hardware::Persiana (int Sentido)
{
   Sentido = -Sentido;

   for (int n = 0; n < 10; n++) {
     DrawFondoBuffer();
     for (int m = 0; m < AltoPantalla; m += 10) {
         if (Sentido < 0)
           rectfill (Buffer, 0, m, AnchoPantalla, m+n, 0);
          else
           rectfill (Buffer, 0, m, AnchoPantalla, m + 10 - n, 0);
      }
      VuelcaBuffer ();
      rest (50);
   }
   if (Sentido > 0)
     DrawFondoBuffer();
}

void Hardware::Persiana2 (int Sentido)
{
     int Ancho = 4;
     int Columnas = AnchoPantalla / Ancho + 1;
     int CActivas = Columnas;
     int Velocidad[Columnas];
     int Avance[Columnas];

     for (int n = 0; n < Columnas; n++) {
         Velocidad[n] = ((int) rand() % 20);
         if (Sentido > 0)
            Avance[n] = 0;
          else
            Avance[n] = AltoPantalla << 7;
         }

     CopyFondo();

     while (CActivas > 0 && !key[KEY_ESC]) {
           cls (0);
           for (int n = 0; n < Columnas; n++) {
               Avance[n] += Velocidad[n] * Sentido;
               if (Sentido > 0 && Avance[n] >> 7 > AltoPantalla)
                  Avance[n] = AltoPantalla << 7;
               if (Sentido < 0 && Avance[n] < 0)
                  Avance[n] = 0;
               blit (Fondo, Buffer,
                     n * Ancho, 0,
                     BufMargX + n * Ancho, BufMargY + Avance[n] >> 7,
                     Ancho, AltoPantalla);
               if (Velocidad[n] >> 7 < 8)
                  Velocidad[n] += ((int) rand() % 3) << 1;
               if ((Sentido > 0 && Avance[n] >> 7 >= AltoPantalla) ||
                   (Sentido < 0 && Avance[n] <= 0))
                  CActivas--;
               }
           VuelcaBuffer();
           }
}

void Hardware::Mamporro (int Cuanto)
{
     if (!Mamporrazo && !MObjetivo)
        MObjetivo = Cuanto;
//     MObjetivo = Mamporrazo / -2;
     return;
}

void Hardware::Terremoto (int Cuanto, bool cls)
{
    Terremot = Cuanto;
    if (Cuanto == 0 && cls) {
        clear_bitmap (screen);
        VuelcaBuffer();
    }
    return;
}

void Hardware::InitJoy ()
{
     if (install_joystick(JOY_TYPE_AUTODETECT) != 0 || !num_joysticks ||
	    (joy[0].stick[0].axis[0].pos/40 < 0 && joy[0].stick[0].axis[1].pos/40 < 0)) {
         LogWrite((char*)"No se detecta joystick.\nContinuando inicializaciÃ³n de Allegro...");
         remove_joystick();
         }
     Config->ToggleJoy (2); // Si se puede, pon que se pueda controlar con Joystick y Teclado
}

void Hardware::Overlay (int R, int G, int B, int Factor)
{
#ifdef FBLEND
    fblend_rect_add(Buffer, 0, 0, Buffer->w, Buffer->h, makecol (R,G,B), Factor);
    return;
#endif
#ifndef FBLEND
    drawing_mode (DRAW_MODE_TRANS,NULL,0,0);
    set_add_blender(0,0,0,Factor);
    rectfill (Buffer, 0, 0, Buffer->w, Buffer->h, makecol (R,G,B));
    solid_mode();
    return;
#endif
}

void Hardware::DrawRotate (void *Sprite, int x, int y, int angle, float scale)
{
    pivot_scaled_sprite (Buffer, (BITMAP *)Sprite, BufMargX + x, BufMargY + y, ((BITMAP *)Sprite)->w / 2, ((BITMAP *)Sprite)->h / 2, itofix (angle), ftofix(scale));
}

void Hardware::Blit (void *Source, void *BitMap, int x, int y, int dx, int dy, int w, int h)
{
    masked_blit ((BITMAP*) Source, (BITMAP*) BitMap, x, y, dx, dy, w, h);
}

