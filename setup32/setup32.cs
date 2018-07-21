using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using Microsoft.Win32;
using WixSharp;
using WixSharp.CommonTasks;


class Script
{
    static public void Main(string[] args)
    {
        try
        {
            Feature binaries = new Feature("binaries");
            var SRSetup = new ManagedProject("SurfaceReader32",
    new Dir(new Id("ExeFolder"), @"%ProgramFilesFolder%\Raised Bar\SurfaceReader",
    new WixSharp.Files(binaries, @"..\Files\Program Files\*.*"),
    new WixSharp.File(binaries, @"..\Files\X86\SurfaceReader.exe",
                                                                            new FileShortcut(binaries, "SurfaceReader", @"%Desktop%")),
            new ExeFileShortcut(binaries, "Uninstall SurfaceReader", "[SystemFolder]msiexec.exe", "/x [ProductCode]"),

            // Configuration files
            new Dir(new Id("DataFiles"), @"%CommonAppDataFolder%\Raised Bar\SurfaceReader",
            new DirPermission("Everyone", GenericPermission.All),
            new Dir(new Id("AppConfigs"), "Apps",
new DirPermission("Everyone", GenericPermission.All),
                        new WixSharp.Files(@"..\Files\ProgramData\Apps\*.*")),
            new Dir(new Id("Protocols"), "Protocols",
new DirPermission("Everyone", GenericPermission.All),
                        new WixSharp.Files(@"..\Files\ProgramData\Protocols\*.*")),
            new Dir(new Id("Surfaces"), "Surfaces",
            new DirPermission("Everyone", GenericPermission.All)))),

                        // Start menu and shortcuts
                        new Dir(@"%ProgramMenu%\Raised Bar\SurfaceReader",
                                                new WixSharp.ExeFileShortcut("SurfaceReader", "[ExeFolder]surfacereader.exe", ""),
                                                new WixSharp.ExeFileShortcut("SurfaceReader Help", "[ExeFolder]surfacereader.chm", ""),
                                                    new WixSharp.ExeFileShortcut(binaries, "Uninstall SurfaceReader", "[SystemFolder]msiexec.exe", "/x [ProductCode]")));

            if (args.Length != 0)
            {
                switch (args[0])
                {
                    case "ProgressOnly": SRSetup.UI = WUI.WixUI_ProgressOnly; break;
                    case "Minimal": SRSetup.UI = WUI.WixUI_Minimal; break;
                    case "InstallDir": SRSetup.UI = WUI.WixUI_InstallDir; break;
                    case "FeatureTree": SRSetup.UI = WUI.WixUI_FeatureTree; break;
                    case "Mondo": SRSetup.UI = WUI.WixUI_Mondo; break;
                }
            }

            SRSetup.Platform = Platform.x86;
            SRSetup.LicenceFile = @"..\Files\License.rtf";
            SRSetup.GUID = new Guid("4334E5FD-9389-4373-B6EB-86CF7473F874");
            SRSetup.InstallPrivileges = InstallPrivileges.elevated;
            SRSetup.OutFileName = "SurfaceReader32";

            Compiler.BuildMsi(SRSetup);
        }
        catch (System.Exception ex)
        {
            Console.WriteLine(ex.Message);
        }
    }
}