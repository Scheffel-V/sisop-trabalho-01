#include "cthread.h"
#include "support.h"
#include "cdata.h"

static int WORLD_THREAD_ID = 0;

int generateThreadID() {
  WORLD_THREAD_ID++;
  printf("[DEBUGANDO] Gerado o ID %d para a thread.", WORLD_THREAD_ID);
  return WORLD_THREAD_ID;
}


//  start: ponteiro para a função que a thread executará.
//  arg: um parâmetro que pode ser passado para a thread na sua criação.
//  prio: NÃO utilizado neste semestre, deve ser sempre zero.
// return: Quando executada corretamente: retorna um valor positivo,
// que representa o identificador da thread criada. Caso contrário,
// retorna um valor negativo.
int ccreate (void *(*start)(void *), void *arg, int prio) {
  TCB_t *thread;

  printf("[DEBUGANDO] Alocando espaço para a thread.");
  thread = malloc(sizeof(TCB_t));
  if(thread == NULL) {
    printf("[DEBUGANDO] Alocação mal sucedida.");
    return -1;
  } else {
    printf("[DEBUGANDO] Alocação bem sucedida.");
  }

  thread->tid = generateThreadID();
  thread->state = PROCST_CRIACAO;

  printf("[DEBUGANDO] Alocando espaço para o ucontext_t da thread %d.", thread->tid);
  thread->ucontext_t = malloc(sizeof(ucontext_t));
  if(thread->ucontext_t == NULL) {
    printf("[DEBUGANDO] Alocação mal sucedida.");
    return -1;
  } else {
    thread->ucontext_t->uc_sigmask = malloc(sizeof(sigset_t));
    if(thread->ucontext_t->uc_sigmask == NULL) {
      printf("[DEBUGANDO] Alocação mal sucedida.");
      return -1;
    } else {
      thread->ucontext_t->uc_stack = malloc(sizeof(stack_t));
      if(thread->ucontext_t->ucstack == NULL) {
        printf("[DEBUGANDO] Alocação mal sucedida.");
        return -1;
      } else {
        thread->ucontext_t->uc_mcontext = malloc(sizeof(mcontext_t));
        if(thread->ucontext_t->uc_mcontext == NULL) {
          printf("[DEBUGANDO] Alocação mal sucedida");
          return -1;
        } else {
          printf("[DEBUGANDO] Alocação bem sucedida.");
        }
      }
    }
    printf("[DEBUGANDO] Alocação bem sucedida.");
  }


  printf("[DEBUGANDO] Alocando coisas a ver com a pilha da thread %d.", thread->tid);
  char  *stack = malloc(sizeof(char)*50000);
  if(stack == NULL) {
    printf("[DEBUGANDO] Alocação mal sucedida.");
    return -1;
  } else {
    thread->ucontext_t->uc_stack->ss_sp = stack;
    thread->ucontext_t->uc_stack->ss_size = sizeof(stack);
    thread->ucontext_t->uc_link = &(thread_main->ucontext_t);
    printf("[DEBUGANDO] Alocação bem sucedida.");
  }


  printf("[DEBUGANDO] Criando o contexto para a thread %d.", thread->tid);
  if(arg == NULL) {
    makecontext(&(thread->ucontext_t), start, 0);
  } else {
    makecontext(&(thread->ucontext_t), start, 1, *(arg));
  }
  printf("[DEBUGANDO] Contexto da thread %d criado com sucesso.", thread->tid);

}
