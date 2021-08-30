#include <GL/glut.h>
#include <vector>
#include <ctime>
#include <iostream>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>		// inet_aton
#include <pthread.h>

#include <signal.h>
#include <errno.h>
#include <time.h>

#include "Canvas/gl_canvas2d.h"
#include "Interface/Fps.h"
#include "Interface/Engine.cpp"
#include "Interface/Botao.h"
#include "Interface/Checkbox.h"
#include "Interface/Panel.h"

// variavel global para selecao do que sera exibido na canvas.
int screenWidth = 1280, screenHeight = 720;  // largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int mouseX, mouseY, old_mouseX, old_mouseY;  // variaveis globais do mouse para poder exibir dentro da render().
bool moving = false;
clock_t timer;

static int thread_red_count;
static int thread_green_count;
static int thread_blue_count;

Fps *fps;
Engine *eng, *eng2;
Botao *less_tooth, *more_tooth, *less_radius, *more_radius, *less_tsize, *more_tsize, *less_speed, *more_speed, *less_thick, *more_thick;
Checkbox *cb_vertical, *cb_horizontal;
Panel *panel;

int sockfd;

void *leitura(void *arg) {
    char buffer[256];
    int n;
    while (true) {
        bzero(buffer, sizeof(buffer));
        n = recv(sockfd, buffer, 50, 0);
        if (n <= 0) {
            std::cout << "Erro lendo do socket!\n";
            exit(1);
        }
        std::cout << "Recebido: " << buffer << "\n";

        char* chars_array = strtok(buffer, "#");
        std::vector<char*> str;
        while(chars_array) {
            str.push_back(chars_array);
            chars_array = strtok(NULL, "#");
        }

        int id;
        double v1, v2;
        sscanf(str[0], "%d", &id);
        sscanf(str[1], "%lf", &v1);
        sscanf(str[2], "%lf", &v2);

        std::cout << "Atualizando dados...\n";
        switch (id) {
            case 1:
                eng->set_n(v1, eng2);
                eng2->set_n(v2, eng);
                break;
            case 2:
                eng->set_r(v1, eng2);
                eng2->set_r(v2, eng);
                break;
            case 3:
                eng->set_r2(v1, eng2);
                eng2->set_r2(v2, eng);
                break;
            case 4:
                eng->set_speed(v1, eng2);
                eng2->set_speed(v2, eng);
                break;
            case 5:
                eng->set_thick(v1, eng2);
                eng2->set_thick(v2, eng);
                break;
        }
        std::cout << "Dados atualizados!\n";
    }
}

struct periodic_info
{
    int sig;
    sigset_t alarm_sig;
};

static int make_periodic (int unsigned period, struct periodic_info *info)
{
    static int next_sig;
    int ret;
    unsigned int ns;
    unsigned int sec;
    struct sigevent sigev;
    timer_t timer_id;
    struct itimerspec itval;

    /* Initialise next_sig first time through. We can't use static
       initialisation because SIGRTMIN is a function call, not a constant */
    if (next_sig == 0)
        next_sig = SIGRTMIN;
    /* Check that we have not run out of signals */
    if (next_sig > SIGRTMAX)
        return -1;
    info->sig = next_sig;
    next_sig++;
    /* Create the signal mask that will be used in wait_period */
    sigemptyset (&(info->alarm_sig));
    sigaddset (&(info->alarm_sig), info->sig);

    /* Create a timer that will generate the signal we have chosen */
    sigev.sigev_notify = SIGEV_SIGNAL;
    sigev.sigev_signo = info->sig;
    sigev.sigev_value.sival_ptr = (void *) &timer_id;
    ret = timer_create (CLOCK_MONOTONIC, &sigev, &timer_id);
    if (ret == -1)
        return ret;

    /* Make the timer periodic */
    sec = period/1000000;
    ns = (period - (sec * 1000000)) * 1000;
    itval.it_interval.tv_sec = sec;
    itval.it_interval.tv_nsec = ns;
    itval.it_value.tv_sec = sec;
    itval.it_value.tv_nsec = ns;
    ret = timer_settime (timer_id, 0, &itval, NULL);
    return ret;
}

static void wait_period(struct periodic_info *info)
{
    int sig;
    sigwait(&(info->alarm_sig), &sig);
}

static void *thread_red(void *arg)
{
    struct periodic_info info;
    std::cout << "Thread red period 100ms\n";
    make_periodic (100000, &info);
    while (true) {
        thread_red_count++;
        wait_period(&info);
    }
}

static void *thread_green (void *arg)
{
    struct periodic_info info;
    std::cout << "Thread green period 200ms\n";
    make_periodic (200000, &info);
    while (true) {
        thread_green_count++;
        wait_period (&info);
    }
}

static void *thread_blue (void *arg)
{
    struct periodic_info info;
    std::cout << "Thread blue period 300ms\n";
    make_periodic (300000, &info);
    while (true) {
        thread_blue_count++;
        wait_period (&info);
    }
}

void render() {
    // Set timer to calculate the FPS after
    timer = clock();

    // Reset background color
    glClearColor(sin((thread_red_count * PI) / 180), sin((thread_green_count * PI) / 180), sin((thread_blue_count * PI) / 180), 0);

    // If moving send how much moved to the gears
    if (moving) {
        eng->add(float(mouseX - old_mouseX), float(mouseY - old_mouseY));
        eng2->add(float(mouseX - old_mouseX), float(mouseY - old_mouseY));
    }

    // Checkbox render
    cb_vertical->Render();
    cb_horizontal->Render();

    // Gear renders
    eng->Render(eng2, cb_vertical->getStatus());
    eng2->Render(eng, cb_vertical->getStatus());

    // Button renders
    less_tooth->Render();
    more_tooth->Render();
    less_radius->Render();
    more_radius->Render();
    less_tsize->Render();
    more_tsize->Render();
    less_speed->Render();
    more_speed->Render();
    less_thick->Render();
    more_thick->Render();

    // Panel render
    panel->Render(mouseX, mouseY);

    // FPS render
    fps->Render(timer);
}

void keyboard(int key) {}

void keyboardUp(int key) {}

// funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y) {
    mouseX = x;  // guarda as coordenadas do mouse para exibir dentro da render()
    mouseY = y;
    char buffer[256];
    int n;

    if (button == 0) {
        if (state == 0) {
            if (less_tooth->Colidiu(x, y)) {
                less_tooth->updateStatus();
                sprintf(buffer, "1#0#%lf#%lf", eng->get_n(), eng2->get_n());
                std::cout << "Enviando: " << buffer << "\n";
                n = send(sockfd, buffer, 50, 0);
            } else if (more_tooth->Colidiu(x, y)) {
                more_tooth->updateStatus();
                sprintf(buffer, "1#1#%lf#%lf", eng->get_n(), eng2->get_n());
                std::cout << "Enviando: " << buffer << "\n";
                n = send(sockfd,buffer,50,0);
            } else if (more_radius->Colidiu(x, y)) {
                more_radius->updateStatus();
                sprintf(buffer, "2#0#%lf#%lf", eng->get_r(), eng2->get_r());
                std::cout << "Enviando: " << buffer << "\n";
                n = send(sockfd, buffer, 50, 0);
            } else if (less_radius->Colidiu(x, y)) {
                less_radius->updateStatus();
                sprintf(buffer, "2#1#%lf#%lf", eng->get_r(), eng2->get_r());
                std::cout << "Enviando: " << buffer << "\n";
                n = send(sockfd, buffer, 50, 0);
            } else if (more_tsize->Colidiu(x, y)) {
                more_tsize->updateStatus();
                sprintf(buffer, "3#0#%lf#%lf", eng->get_r2(), eng2->get_r2());
                std::cout << "Enviando: " << buffer << "\n";
                n = send(sockfd, buffer, 50, 0);
            } else if (less_tsize->Colidiu(x, y)) {
                less_tsize->updateStatus();
                sprintf(buffer, "3#1#%lf#%lf", eng->get_r2(), eng2->get_r2());
                std::cout << "Enviando: " << buffer << "\n";
                n = send(sockfd, buffer, 50, 0);
            } else if (less_speed->Colidiu(x, y)) {
                less_speed->updateStatus();
                sprintf(buffer, "4#0#%lf#%lf", eng->get_speed(), eng2->get_speed());
                std::cout << "Enviando: " << buffer << "\n";
                n = send(sockfd, buffer, 50, 0);
            } else if (more_speed->Colidiu(x, y)) {
                more_speed->updateStatus();
                sprintf(buffer, "4#1#%lf#%lf", eng->get_speed(), eng2->get_speed());
                std::cout << "Enviando: " << buffer << "\n";
                n = send(sockfd, buffer, 50, 0);
            } else if (less_thick->Colidiu(x, y)) {
                less_thick->updateStatus();
                sprintf(buffer, "5#0#%lf#%lf", eng->get_thick(), eng2->get_thick());
                std::cout << "Enviando: " << buffer << "\n";
                n = send(sockfd, buffer, 50, 0);
            } else if (more_thick->Colidiu(x, y)) {
                more_thick->updateStatus();
                sprintf(buffer, "5#1#%lf#%lf", eng->get_thick(), eng2->get_thick());
                std::cout << "Enviando: " << buffer << "\n";
                n = send(sockfd, buffer, 50, 0);
            } else if (x > 0 && x < 1000 && y > 0 && y < 720) {
                moving = true;
                old_mouseX = mouseX;
                old_mouseY = mouseY;
            } else if (cb_vertical->Colidiu(x, y) || cb_horizontal->Colidiu(x, y)) {  // Atualiza o status da checkbox
                cb_vertical->updateStatus();
                cb_horizontal->updateStatus();
                eng->reset_angle();
                eng2->reset_angle();
            }
        } else if (state == 1) {
            if (less_tooth->Colidiu(x, y))
                less_tooth->updateStatus();
            else if (more_tooth->Colidiu(x, y))
                more_tooth->updateStatus();
            else if (more_radius->Colidiu(x, y))
                more_radius->updateStatus();
            else if (less_radius->Colidiu(x, y))
                less_radius->updateStatus();
            else if (more_tsize->Colidiu(x, y))
                more_tsize->updateStatus();
            else if (less_tsize->Colidiu(x, y))
                less_tsize->updateStatus();
            else if (less_speed->Colidiu(x, y))
                less_speed->updateStatus();
            else if (more_speed->Colidiu(x, y))
                more_speed->updateStatus();
            else if (less_thick->Colidiu(x, y))
                less_thick->updateStatus();
            else if (more_thick->Colidiu(x, y))
                more_thick->updateStatus();
            else {
                moving = false;
                eng->save_status();
                eng2->save_status();
            }
        }
    }
}

int main(int argc, char *argv[]) {
    sigset_t alarm_sig;
    pthread_t t_r;
    pthread_t t_g;
    pthread_t t_b;
    int i;

    std::cout << "Periodic threads using POSIX timers\n";
    sigemptyset (&alarm_sig);
    for (i = SIGRTMIN; i <= SIGRTMAX; i++)
        sigaddset (&alarm_sig, i);
    sigprocmask (SIG_BLOCK, &alarm_sig, NULL);

    pthread_create(&t_r, NULL, thread_red, NULL);
    pthread_create(&t_g, NULL, thread_green, NULL);
    pthread_create(&t_b, NULL, thread_blue, NULL);

    int portno;
    struct sockaddr_in serv_addr;
    pthread_t t;

    if (argc < 3) {
        std::cout << "Erro, porta nao definida!\n";
        std::cout << "./cliente PORTA\n";
        exit(1);
    }

    portno = atoi(argv[2]);
    std::cout << "Criando socket para a porta: " << argv[2] << "\n";
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        std::cout << "Erro criando socket!\n";
        return -1;
    }
    std::cout << "Socket criado com sucesso!\n";
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    inet_aton(argv[1], &serv_addr.sin_addr);
    serv_addr.sin_port = htons(portno);
    std::cout << "Se conectando a porta: " << argv[1] << "\n";
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        std::cout << "Erro conectando!\n";
        return -1;
    }
    std::cout << "Conectado com sucesso!\n";
    std::cout << "Iniciando a thread de leitura...\n";
    pthread_create(&t, NULL, leitura, NULL);
    std::cout << "Thread iniciada com sucesso!\n";


    CV::init(&screenWidth, &screenHeight, "Gear");

    // Gears
    eng = new Engine(70, 10, 20, 445, 295, 0.005, 25);
    eng2 = new Engine(70, 10, 20, 555, 425, -0.005, 25);

    // FPS
    fps = new Fps(30, 690);

    // Buttons
    less_tooth = new Botao(1050, 550, 80, 40, const_cast<char*>("-Tooth"));
    more_tooth = new Botao(1150, 550, 80, 40, const_cast<char*>("+Tooth"));
    less_radius = new Botao(1050, 500, 80, 40, const_cast<char*>("-Radius"));
    more_radius = new Botao(1150, 500, 80, 40, const_cast<char*>("+Radius"));
    less_tsize = new Botao(1050, 450, 80, 40, const_cast<char*>("-TSize"));
    more_tsize = new Botao(1150, 450, 80, 40, const_cast<char*>("+TSize"));
    less_thick = new Botao(1050, 400, 80, 40, const_cast<char*>("-Thick"));
    more_thick = new Botao(1150, 400, 80, 40, const_cast<char*>("+Thick"));
    less_speed = new Botao(1050, 350, 80, 40, const_cast<char*>("-Speed"));
    more_speed = new Botao(1150, 350, 80, 40, const_cast<char*>("+Speed"));

    // Checkbox
    cb_vertical = new Checkbox(1050, 270, 40, 40, const_cast<char*>("Vertical"), true);
    cb_horizontal = new Checkbox(1050, 220, 40, 40, const_cast<char*>("Horizontal"), false);

    // Panel
    panel = new Panel();

    CV::run();
}
