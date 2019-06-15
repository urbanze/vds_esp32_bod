extern "C"
{
  #include <driver/rtc_cntl.h>
}

#define LED 2

void setup()
{
  pinMode(LED, OUTPUT);

  
  init_bod();
}

void loop()
{
  
  
}

void IRAM_ATTR bod_isr(void *z)
{
    //Liga o LED
    digitalWrite(LED, 1);

    //Mostra no Serial monitor. Voce nao conseguira ler nada se remover o cabo USB
    ets_printf("\nBOD Trigged (2.8V +- 50mV)\n", 1);

    //Aborta a execucao, forcando o reinicio
    abort();
}

void init_bod()
{
  uint32_t value = 0;
  value |= (1<<30);//Ativa o BOD
  value |= (2<<16);//Ciclos de espera, precisa ser >1 & <1024. Padrao 2
  value |= (1<<15);//Desativa o RF
  value |= (7<<27);//Nivel de trigger(2.8V). Para 2.43V ficaria (0<<27)
  

  //Configura o registrador do BOD
  (*(volatile uint32_t *)(0x3FF480D4)) |= value;

  //Configura a ISR (bod_isr) pro BOD
  rtc_isr_register(bod_isr, NULL, (1<<7));

  //Habilita a interrupcao do BOD
  (*(volatile uint32_t *)(0x3FF4803C)) |= (1<<7);
}
