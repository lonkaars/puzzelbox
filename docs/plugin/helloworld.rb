class HelloWorldMacro < Asciidoctor::Extensions::BlockMacroProcessor
	use_dsl
	named :helloworld

	def process parent, target, attrs
    return
	end
end

Asciidoctor::Extensions.register do
	block_macro HelloWorldMacro
end

