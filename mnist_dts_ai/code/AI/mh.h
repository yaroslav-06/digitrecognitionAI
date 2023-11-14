struct shp{
    int w, h, l = 1;
};


struct dts{
    int ans;
    float *bt;
    
};


struct ai_dt{
    int len;
    shp sz;
    dts *dt;
};

void out_dts(dts &dt, shp sz);
