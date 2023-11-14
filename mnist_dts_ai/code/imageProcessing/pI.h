const int p_side = 28, p_area = 5408;
struct pic{
    char ans;
    unsigned char rs, cs;
    char pix[p_area];
};

struct pics{
    pic *photos;
    int len;
};

pics readPict(bool t);
