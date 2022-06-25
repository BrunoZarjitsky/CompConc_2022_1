from LeClass import LE

DEBUG = True


class ScriptRunnerClass:
	def __init__(self, text):
		self.le = LE()
		self.list_of_instructions = text.split("\n")
		self.list_of_checks = []
		self.list_of_success = []
		self.list_of_error = []
		self.error_count = 0
		self.success_count = 0

		self.actual_instruction = ""
		self.previus_instruction = ""
		self.next_instruction = ""

		self.even_or_odd = ["---PAR---", "---IMPAR---"]

	def get_size_of_instruction_list(self) -> int:
		return len(self.list_of_instructions)

	def get_instruction_from_list(self, id_of_instruction: int = None) -> str:
		list_size = self.get_size_of_instruction_list()

		if id_of_instruction == 0:
			self.previus_instruction = None
		else:	
			self.previus_instruction = self.actual_instruction

		self.actual_instruction = self.list_of_instructions[id_of_instruction]

		increment_to_next_valid = 1
		while id_of_instruction + increment_to_next_valid < list_size :
			self.next_instruction = self.list_of_instructions[id_of_instruction + increment_to_next_valid]
			if "Bloqueado" in self.next_instruction:
				increment_to_next_valid += 1
				self.next_instruction = None
			else:
				break

		return self.actual_instruction

	def execute_instruction(self):
		try:
			# Verifica se a T3 esta imprimindo o numero corretamente
			int(self.actual_instruction)
			if int(self.actual_instruction) in self.le.numbers_to_print:
				self.le.numbers_to_print.remove(int(self.actual_instruction))
				self.approve_instruction()
				return True
		except:
			pass

		if self.actual_instruction == "": # Verifica se a instrucao esta vazia
			self.approve_instruction()
		elif self.actual_instruction in ['---PAR---', '---IMPAR---']: # Valida o print de ---PAR--- e ---IMPAR---
			self.le.decrease_even_or_odd(self.actual_instruction)
			self.approve_instruction()
		else: # Tenta executar os outros tipos de instrucoes
			try:
				eval(self.actual_instruction)
				if self.le.expected_instruction != "" and self.le.expected_instruction not in self.even_or_odd:
					if self.le.compare_expected_instruction(self.next_instruction):
						self.approve_instruction()
						self.le.expected_instruction = ""
					else:
						self.reprove_instruction(self.le.expected_instruction)
						self.le.expected_instruction = ""
				else:
					self.approve_instruction()
			except Exception as error:
				self.reprove_instruction(error)

	def approve_instruction(self):
		self.success_count += 1
		data = {
				"status": "SUCCESS",
				"instruction": self.actual_instruction,
			}
		self.list_of_success.append(data)
		self.list_of_checks.append(data)
	
	def reprove_instruction(self, error = None):
		self.error_count += 1
		data = {
				"status": "ERROR",
				"instruction": self.actual_instruction,
				"details": error,
			}
		self.list_of_error.append(data)
		self.list_of_checks.append(data)

	def get_instruction_parameters(self, instruction):
		opening_position = instruction.find("(")
		if opening_position == -1:
			return -1
		instruction_arguments = instruction[opening_position + 1 : -1]
		list_of_parameters = instruction_arguments.split(", ")
		return list_of_parameters

	def get_results(self) -> dict:
		self.error_count += self.le.odd_missing + self.le.odd_missing + len(self.le.numbers_to_print)
		data = {
			"cases_of_success": self.success_count,
			"cases_of_failure": self.error_count,
			"list_of_checks": self.list_of_checks,
			"list_of_success": self.list_of_success,
			"list_of_error": self.list_of_error,
		}
		return data

def main():
	text = open('textScript.txt', 'r')
	script_runner = ScriptRunnerClass(text.read())

	for i in range(script_runner.get_size_of_instruction_list()):
		instruction = script_runner.get_instruction_from_list(i)
		parameters = script_runner.get_instruction_parameters(instruction)
		script_runner.execute_instruction()
	
	results = script_runner.get_results()

	if DEBUG:
		for i in results['list_of_error']:
			print(i)
	print(f"Success: {results['cases_of_success']}, Erros: {results['cases_of_failure']}")


if __name__ == "__main__":
	main()