/*
 *  MVPreferenceController.h
 *  MachOView
 *
 *  Created by psaghelyi on 12/24/12.
 *
 */

@interface MVPreferenceController: NSWindowController
{
  IBOutlet NSButton *   useLLVMDisassembler;
}

- (IBAction)toggleUseLLVMDisassembler:(id)sender;

@end