// porres

#include "m_pd.h"

static t_class *sigs_tilde_class;

typedef struct _sigs{
    t_object x_obj;
    t_atom  *x_vec;
    int      x_n;
}t_sigs;

static void sigs_tilde_float(t_sigs *x, t_floatarg f){
    x->x_vec[0].a_w.w_float = f;
}

static void sigs_tilde_dsp(t_sigs *x, t_signal **sp){
    signal_setmultiout(sp, x->x_n);
    for(int i = 0; i < x->x_n; i++)
        dsp_add_scalarcopy(&x->x_vec[i].a_w.w_float,
            sp[0]->s_vec + i * sp[0]->s_n, (t_int)sp[0]->s_n);
}

static void *sigs_tilde_new(t_symbol *s, int ac, t_atom *av){
    s = NULL;
    int i;
    t_sigs *x = (t_sigs *)pd_new(sigs_tilde_class);
    if(ac >= 1){
        x->x_vec = (t_atom *)getbytes(ac * sizeof(*x->x_vec));
        for(i = 0; i < ac; i++)
            SETFLOAT(x->x_vec + i, atom_getfloat(av + i));
        x->x_n = ac;
    }
    else{
        x->x_vec = (t_atom *)getbytes(sizeof(*x->x_vec));
        SETFLOAT(x->x_vec, 0);
        x->x_n = 1;
    }
    for(i = 1; i < x->x_n; i++)
        floatinlet_new(&x->x_obj, &x->x_vec[i].a_w.w_float);
    outlet_new(&x->x_obj, gensym("signal"));
    return(x);
}

static void sigs_tilde_free(t_sigs *x){
    freebytes(x->x_vec, x->x_n * sizeof(*x->x_vec));
}

void sigs_tilde_setup(void){
    sigs_tilde_class = class_new(gensym("sigs~"), (t_newmethod)sigs_tilde_new,
        (t_method)sigs_tilde_free, sizeof(t_sigs), CLASS_MULTICHANNEL, A_GIMME, 0);
    class_addfloat(sigs_tilde_class, (t_method)sigs_tilde_float);
    class_addmethod(sigs_tilde_class, (t_method)sigs_tilde_dsp, gensym("dsp"), A_CANT, 0);
}
