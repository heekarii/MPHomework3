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
	printf("%.2f sec", (end - start) / (double) 1000);
	cvShowImage("src", src);
	cvShowImage("dst", dst);
	cvWaitKey();
	return 0;
}

void myFastestMeanFilter(IplImage *src, IplImage *dst, int K) {
	const int sizeX = src->width;
	const int sizeY = src->height;

	// ��ü �ȼ��� ���Ͽ� ä�κ��� bright �� ����


	CvScalar **prefix = (CvScalar **)malloc(sizeY * sizeof(CvScalar *));
	for (int i = 0; i < sizeY; i++)
		prefix[i] = (CvScalar *)malloc(sizeX * sizeof(CvScalar));

	// ��ü �ȼ��� ���� ������ ���
	for (int y = 0;  y  < src->height; y++)
		for (int x = 0; x < src->width; x++) {
			CvScalar f = cvGet2D(src, y, x);
			// ���� �ȼ� �� ����
			prefix[y][x] = cvGet2D(src, y, x);

			// �»��->���ϴ� �������� �ȼ� �̵� ����			
			if (y > 0) {
				prefix[y][x].val[0] += prefix[y - 1][x].val[0];
				prefix[y][x].val[1] += prefix[y - 1][x].val[1];
				prefix[y][x].val[2] += prefix[y - 1][x].val[2];
			}
			if (x > 0) {
				prefix[y][x].val[0] += prefix[y][x - 1].val[0];
				prefix[y][x].val[1] += prefix[y][x - 1].val[1];
				prefix[y][x].val[2] += prefix[y][x - 1].val[2];
			}
			if (y > 0 && x > 0) {
				prefix[y][x].val[0] -= prefix[y - 1][x - 1].val[0];
				prefix[y][x].val[1] -= prefix[y - 1][x - 1].val[1];
				prefix[y][x].val[2] -= prefix[y - 1][x - 1].val[2];
			}
		}
	// filtering
	for (int y = 0; y < src->height; y++)
		for (int x = 0; x < src->width; x++) {
			// kernel size ����
			int x1 = x - K;
			int y1 = y - K;
			int x2 = x + K;
			int y2 = y + K;

			// �׵θ��� padding�� �����ϱ� ���� ����(��ȿ���� ���� ���� ���� ��� ��ȿ�� ������ �ٲ��־���)
			if (x1 < 0)						x1 = 0;
			if (y1 < 0)						y1 = 0;
			if (x2 > src->width - 1)		x2 = src->width - 1;
			if (y2 > src->height - 1)		y2 = src->height - 1;
			// ���� ������ ũ��
			const int valid = (x2 - x1 + 1) * (y2 - y1 + 1);
			
			// window ���� ���ϴ� �ȼ���
			double b = prefix[y2][x2].val[0];
			double g = prefix[y2][x2].val[1];
			double r = prefix[y2][x2].val[2];

			// SAT
			if (x1 > 0) {
				b -= prefix[y2][x1 - 1].val[0];
				g -= prefix[y2][x1 - 1].val[1];
				r -= prefix[y2][x1 - 1].val[2];
			}
			if (y1 > 0) {
				b -= prefix[y1 - 1][x2].val[0];
				g -= prefix[y1 - 1][x2].val[1];
				r -= prefix[y1 - 1][x2].val[2];
			}
			if (x1 > 0 && y1 > 0) {
				b += prefix[y1 - 1][x1 - 1].val[0];
				g += prefix[y1 - 1][x1 - 1].val[1];
				r += prefix[y1 - 1][x1 - 1].val[2];
			}

			// ��հ����� �̹��� ä���
			CvScalar mean = cvScalar(b / valid, g / valid, r / valid);
			cvSet2D(dst, y, x, mean);
		}
}