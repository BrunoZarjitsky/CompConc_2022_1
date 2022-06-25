#Define funcoes para verificar a logica de execucao de uma aplicacao leitor/escritor
class LE:
	def __init__(self):
		self.escritores = 0
		self.leitores = 0
		self.expected_instruction = ""
		self.even_missing = 0
		self.odd_missing = 0
		self.numbers_to_print = []

	def leitorBloqueado(self, id, thread_type, control_var):
		'''Recebe o id do leitor. Verifica se a decisao de bloqueio esta correta.'''
		if(self.escritores==0):
			print("ERRO: leitor " + str(id) + " bloqueado quando nao ha escritores!")

	def escritorBloqueado(self, id, thread_type, control_var):
		'''Recebe o id do escritor. Verifica se a decisao de bloqueio esta correta.'''
		if(self.escritores==0) and (self.leitores==0):
			print("ERRO: escritor " + str(id) + " bloqueado quando nao ha escritores nem leitores!")

	def leitorLendo(self, id, thread_type, control_var):
		'''Recebe o id do leitor, verifica se pode ler e registra que esta lendo.'''
		if(self.escritores>0):
			print("ERRO: leitor " + str(id) + " esta lendo quando ha escritor escrevendo!")
		self.leitores+=1
		if thread_type == 2:
			if control_var % 2 == 0:
				self.even_missing += 1
			else:
				self.odd_missing += 1
		elif thread_type == 3:
			self.numbers_to_print.append(control_var)

	def escritorEscrevendo(self, id, thread_type, control_var):
		'''Recebe o id do escritor, verifica se pode escrever e registra que esta escrevendo'''
		if(self.escritores>0 or self.leitores>0):
			print("ERRO: escritor " + str(id) + " esta escrevendo quando ha outro escritor ou leitores!")
		self.escritores+=1
		if thread_type == 1:
			self.expected_instruction = f"self.le.escritorSaindo({id}, {thread_type}, {int(control_var)+1})"
		elif thread_type == 3:
			self.expected_instruction = f"self.le.escritorSaindo({id}, {thread_type}, {id})"

	def leitorSaindo(self, id, thread_type, control_var):
		'''Recebe o id do leitor e registra que terminou a leitura.'''
		self.leitores-=1
		

	def escritorSaindo(self, id, thread_type, control_var):
		'''Recebe o id do escritor e registra que terminou a leitura.'''
		self.escritores-=1

	def compare_expected_instruction(self, text):
		if self.expected_instruction == text:
			return True
		return False

	def decrease_even_or_odd(self, text):
		if "---PAR---" == text:
			self.even_missing -= 1
		elif "---IMPAR---" == text:
			self.odd_missing -= 1