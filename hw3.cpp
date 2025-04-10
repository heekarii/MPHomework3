#include<opencv2/opencv.hpp>

void myFastestMeanFilter(IplImage *src, IplImage *dst, int K);

int main() {
	printf("=============================================\n");
	printf("Software Department, Sejong University\n");
	printf("Multimedia Programming Class\n");
	printf("Homework: Fastest Mean Filter\n");
	printf("=============================================\n\n");
	char path[100];
	int K;
	printf("Input File Path:");
	scanf("%s", path);
	printf("Input Kernel Size:");
	scanf("%d", &K);

	K = (const int)K;
	IplImage *src = cvLoadImage((const char *)path);
	IplImage *dst = cvCreateImage(cvGetSize(src), 8, 3);

	long start = clock();
	myFastestMeanFilter(src, dst, K);
	long end = clock();
	printf("%f sec", (end - start) / (double) 1000);
	cvShowImage("src", src);
	cvShowImage("dst", dst);
	cvWaitKey();
	return 0;
}

void myFastestMeanFilter(IplImage *src, IplImage *dst, int K) {
	const int SIZE = (2 * K + 1) * (2 * K + 1);
	const int sizeX = src->width;
	const int sizeY = src->height;

	// ��ü �ȼ��� ���Ͽ� ä�κ��� bright �� ����
	int **blue  = (int **)malloc(sizeY * sizeof(int *));
	int **green = (int **)malloc(sizeY * sizeof(int *));
	int **red   = (int **)malloc(sizeY * sizeof(int *));
	for (int i = 0; i < sizeY; i++) {
		blue[i]  = (int *)calloc(sizeX, sizeof(int));
		green[i] = (int *)calloc(sizeX, sizeof(int));
		red[i]   = (int *)calloc(sizeX, sizeof(int));
	}

	// ��ü �ȼ��� ���� ������ ���
	for (int y = 0;  y  < src->height; y++)
		for (int x = 0; x < src->width; x++) {
			CvScalar f = cvGet2D(src, y, x);
			// ���� �ȼ� �� ����
			blue[y][x] = (int)f.val[0];
			green[y][x] = (int)f.val[1];
			red[y][x] = (int)f.val[2];

			// �»��->���ϴ� �������� �ȼ� �̵� ����			
			if (y > 0) {
				blue[y][x]  += blue[y - 1][x];
				green[y][x] += green[y - 1][x];
				red[y][x]   += red[y - 1][x];
			}
			if (x > 0) {
				blue[y][x]  += blue[y][x - 1];
				green[y][x] += green[y][x - 1];
				red[y][x]   += red[y][x - 1];
			}
			if (y > 0 && x > 0) {
				blue[y][x]  -= blue[y - 1][x - 1];
				green[y][x] -= green[y - 1][x - 1];
				red[y][x]   -= red[y - 1][x - 1];	
			}
		}
	// filtering
	for (int y = K; y < src->height - K; y++)
		for (int x = K; x < src->width - K; x++) {
			// kernel size ����
			int x1 = x - K;
			int y1 = y - K;
			int x2 = x + K;
			int y2 = y + K;

			// window ���� ���ϴ� �ȼ���
			int b = blue[y2][x2];
			int g = green[y2][x2];
			int r = red[y2][x2];

			// SAT
			if (x1 > 0) {
				b -= blue[y2][x1 - 1];
				g -= green[y2][x1 - 1];
				r -= red[y2][x1 - 1];
			}
			if (y1 > 0) {
				b -= blue[y1 - 1][x2];
				g -= green[y1 - 1][x2];
				r -= red[y1 - 1][x2];
			}
			if (x1 > 0 && y1 > 0) {
				b += blue[y1 - 1][x1 - 1];
				g += green[y1 - 1][x1 - 1];
				r += red[y1 - 1][x1 - 1];
			}

			// ��հ����� �̹��� ä���
			CvScalar mean = cvScalar(b / SIZE, g / SIZE, r / SIZE);
			cvSet2D(dst, y, x, mean);
		}


}
