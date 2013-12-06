require "erb"

OUTPUT_DIR = "test/lib"

class TestTemplate

  attr_accessor :test_methods

  def initialize(dir)
    @test_methods = Dir["#{dir}/**/*_test.c"].map do |tf|
      c = File.read tf
      c.scan /^void (test_.*)\(\)/
    end.flatten.shuffle

    focus = @test_methods.reject { |m| !m.end_with? "_f" }
    @test_methods = focus unless focus.empty?
  end

  def render(template)
    tmpl = File.read template
    ERB.new(tmpl).result(binding)
  end
end

t = TestTemplate.new "./test"
File.open "#{OUTPUT_DIR}/tests.h", "w" do |f|
  f.write t.render("#{OUTPUT_DIR}/tests.h.erb")
end

File.open "#{OUTPUT_DIR}/runner.c", "w" do |f|
  f.write t.render("#{OUTPUT_DIR}/runner.c.erb")
end
