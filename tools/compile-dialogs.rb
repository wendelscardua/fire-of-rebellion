# frozen_string_literal: true

# converts dialogs to assembly

require 'yaml'

input_file, output_file = ARGV

dialogs = YAML.safe_load(File.read(input_file))

dialog_table = {}

characters = {
  'Me' => 1,
  'Someone' => 2
}

def fix(value)
  value.gsub(/([áãéêíó])/) do |match|
    codepoint = {
      'á' => '$5f',
      'ã' => '$62',
      'é' => '$63',
      'ê' => '$64',
      'í' => '$65',
      'ó' => '$66'
    }[match]
    '", ' + codepoint + ', "'
  end
end

dialogs&.each do |dialog_name, dialog_content|
  dialog_table[dialog_name] = []
  dialog_content.each do |dialog_entry|
    key = dialog_entry.keys.first
    value = dialog_entry.values.first
    dialog_table[dialog_name] << characters[key]
    dialog_table[dialog_name] << '"' + fix(value) + '"'
  end
  dialog_table[dialog_name] << 0
end

File.open(output_file, 'wb') do |f|
  f.puts '.include "../src/charmap.inc"'
  f.puts '.segment "RODATA"'

  dialog_table.each do |dialog_name, dialog_content|
    f.puts ".export _dialog_#{dialog_name}"
    f.puts "_dialog_#{dialog_name}: .byte #{dialog_content.join(', ')}"
  end
end
