char Get_Voltage_Data(void);
float Get_Voltage_VL1(void);
float Get_Voltage_VL2(void);
float Get_Voltage_VL3(void);
float Get_Voltage_Vin(void);
float Get_Control_Voltage(void);
int Get_Variables(void);
void Save_Variables(void);
void set_max_VL(float);
void set_min_VL(float);
float get_max_VL(void);
float get_min_VL(void);
void set_max(float);
void set_min(float);
float get_max(void);
float get_min(void);
float get_VL1_zero(void);
float get_VL2_zero(void);
float get_VL3_zero(void);
float get_VL1_span();
float get_VL2_span(void);
float get_VL3_span(void);
float get_Vin_span(void);
float get_Vin_zero(void);
void set_VL1_zero(float val);
void set_VL2_zero(float val);
void set_VL3_zero(float val);
void set_VL1_span(float val);
void set_VL2_span(float val);
void set_VL3_span(float val);
void set_Vin_zero(float val);
void set_Vin_span(float val);

union data_check {
    unsigned char data;

    struct {
        unsigned VL1_Low : 1;
        unsigned VL1_High : 1;
        unsigned VL2_Low : 1;
        unsigned VL2_High : 1;
        unsigned VL3_Low : 1;
        unsigned VL3_High : 1;
        unsigned Vin_Low : 1;
        unsigned Vin_High : 1;
    };

};

#define Array_Size 53
#define CODE 1234
union data_var {
    unsigned char array[Array_Size];
    struct{
        int code;
        float kp;
        float ti;
        float td;
        float T_sample;
        float set_point;
        float deadband;
        float RC_Constant;
        float max_VL;
        float min_VL;
        float V1_zero;
        float V2_zero;
        float V3_zero;
        float V1_span;
        float V2_span;
        float V3_span;
        float Vin_zero;
        float Vin_span;
    };
};